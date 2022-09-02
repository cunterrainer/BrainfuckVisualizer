#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdio>

#include "SFML/Graphics.hpp"

#include "Arial.h"

static constexpr uint16_t sg_ArraySize = 30000;
static const std::string sg_ValidChars = "-+<>[],.";

class SourceCode
{
private:
	static std::string LoadFile(const char* path)
	{
		std::ifstream ifs(path);
		if (!ifs.is_open())
		{
			std::cerr << "Failed to open file [" << path << "]\n";
			return std::string();
		}

		std::string content;
		content.reserve(static_cast<size_t>(ifs.tellg()));
		for (std::istreambuf_iterator<char> it = ifs; it != std::istreambuf_iterator<char>(); ++it)
		{
			if (sg_ValidChars.find(*it) != std::string::npos)
				content += *it;
		}
		return content;
	}
private:
	std::string m_sCode;
	sf::Text m_Text;
	sf::RenderWindow& m_Window;
	const float m_Width;
private:
	void MakeTextFitView()
	{
		for (size_t i = 0; i < m_sCode.size(); ++i)
		{
			if (m_Text.findCharacterPos(i).x > m_Width)
			{
				m_sCode.insert(i-1, "\n");
				m_Text.setString(m_sCode);
			}
		}
	}
public:
	explicit SourceCode(const char* path, const sf::Font& font, sf::RenderWindow& window)
		: m_sCode(LoadFile(path)), m_Text(m_sCode, font, 25), m_Window(window), m_Width(static_cast<float>(m_Window.getSize().x) / 3.f)
	{
		MakeTextFitView();
	}
	char operator[](size_t index) { return m_sCode[index]; }

	std::string& GetString() { return m_sCode; }
	void SetChar(size_t index, char c) { m_sCode[index] = c; m_Text.setString(m_sCode); }
	bool Empty() const { return m_sCode.empty(); }
	void Draw()  const { m_Window.draw(m_Text);  }
	size_t Size() const { return m_sCode.size(); }
};


class Interpreter
{
private:
	uint8_t* arr = new uint8_t[sg_ArraySize]();
	size_t m_IndexArr = 0;
	size_t m_IndexSrc = 0;
	SourceCode& m_sCode;
private:
	size_t AdvanceToEnd(size_t i)
	{
		++i;
		size_t openings = 1;
		for (; openings && i < m_sCode.Size(); i++)
		{
			openings += static_cast<size_t>(m_sCode[i] == '[');
			openings -= static_cast<size_t>(m_sCode[i] == ']');
		}
		if (openings > 0)
		{
			std::cerr << "\nSyntax error: missing ']' for opening bracket('[')\n";
			return std::string::npos;
		}
		return i - 1;
	}

	size_t ResetToBegin(int32_t i)
	{
		--i;
		size_t closings = 1;
		for (; closings && i >= 0; i--)
		{
			closings += static_cast<size_t>(m_sCode[static_cast<size_t>(i)] == ']');
			closings -= static_cast<size_t>(m_sCode[static_cast<size_t>(i)] == '[');
		}
		if (closings > 0)
		{
			std::cerr << "\nSyntax error: missing '[' for closing bracket(']')\n";
			return std::string::npos;
		}
		return static_cast<size_t>(i);
	}
public:
	explicit Interpreter(SourceCode& code) : m_sCode(code) {}
	~Interpreter() { delete[] arr; }

	bool Interpret()
	{
		if (m_IndexSrc == m_sCode.Size())
			return true;

		bool validChar = true;
		switch (m_sCode[m_IndexSrc])
		{
		case '+':
			++arr[m_IndexArr];
			break;
		case '-':
			--arr[m_IndexArr];
			break;
		case '<':
			--m_IndexArr;
			break;
		case '>':
			++m_IndexArr;
			break;
		case ',':
			arr[m_IndexArr] = static_cast<uint8_t>(std::getchar());
			std::fflush(stdout);
			break;
		case '.':
			std::putchar(arr[m_IndexArr]);
			std::fflush(stdout);
			break;
		case '[':
			if (arr[m_IndexArr] == 0)
			{
				m_IndexSrc = AdvanceToEnd(m_IndexSrc);
				if (m_IndexSrc == std::string::npos) return false;
			}
			break;
		case ']':
			if (arr[m_IndexArr] != 0)
			{
				m_IndexSrc = ResetToBegin(static_cast<int32_t>(m_IndexSrc));
				if (m_IndexSrc == std::string::npos) return false;
			}
			break;
		default:
			validChar = false;
			break;
		}

		if (validChar)
		{
			static size_t lastPos = m_IndexSrc;
			static char lastPosChar = m_sCode[m_IndexSrc];

			m_sCode.SetChar(lastPos, lastPosChar);

			lastPos = m_IndexSrc;
			lastPosChar = m_sCode[m_IndexSrc];
			m_sCode.SetChar(lastPos, '|');
		}

		++m_IndexSrc;
		return true;
	}
};


int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Brainfuck Visualizer");
	sf::Font font;
	if (!font.loadFromMemory(sg_RawArialData, sg_RawArialDataRelativeSize))
	{
		std::cerr << "Failed to load font from memory\n";
		return 1;
	}
	SourceCode sCode("prime.txt", font, window);
	if (sCode.Empty()) return 1;
	Interpreter itp(sCode);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (!itp.Interpret())
			return 1;

		window.clear();
		sCode.Draw();
		window.display();
	}
	return 0;
}