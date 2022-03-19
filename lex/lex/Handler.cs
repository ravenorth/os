using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lex
{
    class Handler
    {
        static private Dictionary<string, string> bigTokens = new Dictionary<string, string>()
        {
            {"PROGRAM", "start program token" },
            {"PROCEDURE", "procedure token"},
            {"BEGIN", "Begin token"},
            {"VAR", "declaring variables block token" },
            {"STRING", "variables type token" },
            {"INTEGER", "variables type token" },
            {"BOOLEAN", "variables type token" },
            {"TEXT", "variables type token" },
            {"CHAR", "variables type token" },
            {"END", "end block token" },
            {"IF", "if token" },
            {"THEN", "then token" },
            {"ELSE", "else token" },
            {"TRUE", "true token" },
            {"FALSE", "false token" },
            {"RESET", "reset file token" },
            {"ASSIGN", "assign file token" },
            {"REWRITE", "rewrite file token" },
            {"CLOSE",  "close file token" },
            {"WHILE", "while token" },
            {"FOR", "for token" },
            {"DO", "do token" },
            {"INC", "increment token" },
            {"READ", "read token" },
            {"READLN", "readln token" },
            {"WRITE", "write token" },
            {"WRITELN", "writeln token" },
        };

        static private Dictionary<string, string> oneSymbolTokens = new Dictionary<string, string>()
        {
            {";", "end line token" },
            {":", "declaring variable token" },
            {"=", "equality token" },
            {"(", "open bracket token" },
            {")", "close bracket token" },
            {"+", "plus token" },
            {"-", "minus token" },
            {"*", "multiply token" },
            {">", "more token" },
            {"<", "less token" },
            {"/", "divide token" },
            {",", "split token" },
            {".", "end program token" }
        };

        static private Dictionary<string, string> twoSymbolTokens = new Dictionary<string, string>()
        {
            {"<>", "not equal token" },
            {":=", "equate token" },
            {">=", "more or equal token" },
            {"=<", "less or equal token" }
        };

        const string specSym = "<>:=";

        const string VARIABLE_TOKEN_STR = "variable token";
        const string LITERAL_TOKEN_STR = "literal token";

        struct Token
        {
            public string name;
            public string type;
            public int pos;
            public int line;
        }

        private static string _inFileName;

        public Handler(string inFileName)
        {
            _inFileName = inFileName;
        }

        static List<Token> _tokens = new List<Token>();
        static bool _isCom = false;
        static bool _isComLocal = false;
        static bool _isExit = false;
        static string _bufferCh = "";
        static int _count = 0;

        public void Run()
        {
            StreamReader sr = new StreamReader(_inFileName);
            string input = sr.ReadToEnd();
            string[] inputList = input.Split('\n');
            int line = 0;
            foreach (string str in inputList)
            {
                SearchTokens(str, line);
                line++;
            }

            WriteRes();
        }

        static private void SearchTokens(string str, int line)
        {
            string bufferLit = "";

            _count = 0;
            _bufferCh = "";
            _isComLocal = false;
            _isExit = false;

            for (int i = 0; i < str.Length; i++)
            {
                CheckCom(str[i]);
                if (i + 1 < str.Length)
                {
                    CheckLocalCom(str[i], str[i + 1]);
                    if (!_isCom && !_isComLocal)
                    {
                        _isExit = (((str[i] == '/') && (str[i + 1] == '/'))) ? true : _isExit;
                    }
                }

                if (_isExit)
                {
                    return;
                }

                if (_isComLocal)
                {
                    bufferLit += str[i];
                }
                else
                {
                    if (bufferLit.Length > 1)
                    {
                        Token token = new Token();
                        token.name = bufferLit.Remove(0, 1);
                        token.pos = i - bufferLit.Length + 1;
                        token.line = line;
                        token.type = LITERAL_TOKEN_STR;
                        _tokens.Add(token);
                    }

                    bufferLit = "";
                }

                CheckSym(str[i], ((i == str.Length - 1) ? ' ' : str[i + 1]), ref i, line);
            }

            ToTokens(line);
        }

        static private void ToTokens(int line)
        {
            if (_bufferCh.Length > 0)
            {
                _tokens.Add(GetToken(_bufferCh, line, _bufferCh.Length));
            }
        }

        static private void CheckSym(char ch1, char ch2, ref int i, int line)
        {
            if ((char.IsDigit(ch1) || char.IsLetter(ch1)) && !_isCom && !_isComLocal)
            {
                _bufferCh += ch1.ToString();
            }
            else if ((ch1 != ('\'') && ch1 != ('{') && ch1 != ('/')) && !_isCom && !_isComLocal)
            {
                if (_bufferCh.Length > 0)
                {
                    _tokens.Add(GetToken(_bufferCh, line, i));
                }

                if (!specSym.Contains(ch1) && ch1 != ' ')
                {
                    _tokens.Add(GetToken(ch1.ToString(), line, i + 1));
                }
                else if (ch1 != ' ')
                {
                    if (specSym.Contains(ch2))
                    {
                        _tokens.Add(GetToken((ch1.ToString() + ch2.ToString()).ToString(), line, i + 2));
                        i++;
                    }
                    else
                    {
                        _tokens.Add(GetToken(ch1.ToString(), line, i + 1));
                    }
                }
                _bufferCh = "";
            }
        }

        static private Token GetToken(string name, int line, int pos)
        {
            Token token = new Token();
            token.name = name;
            token.pos = pos - name.Length;
            token.line = line;

            if (bigTokens.ContainsKey(name))
            {
                token.type = bigTokens[name];
                return token;
            }

            if (twoSymbolTokens.ContainsKey(name))
            {
                token.type = twoSymbolTokens[name];
                return token;
            }

            if (oneSymbolTokens.ContainsKey(name))
            {
                token.type = oneSymbolTokens[name];
                return token;
            }

            token.type = VARIABLE_TOKEN_STR;
            return token;
        }

        static private void CheckCom(char ch)
        {
            if ((ch == '{') && (!_isComLocal))
            {
                _isCom = true;
            }

            if ((ch == '}') && (!_isComLocal))
            {
                _isCom = false;
            }
        }

        static private void CheckLocalCom(char ch1, char ch2)
        {
            if (ch1 == '\'')
            {
                _count++;
                _isComLocal = true;
            }

            if ((_count % 2 == 0) && (ch1 == '\''))
            {
                if (ch2 != '\'')
                {
                    _isComLocal = false;
                }
            }
        }

        static private void WriteRes()
        {
            StreamWriter f = new StreamWriter("output.txt");
            foreach (Token token in _tokens)
            {
                string output = "token: " + token.name + "\ttype: " + token.type + "\tpos: " + token.pos + "\tline: " + token.line;
                f.WriteLine(output);
            }

            f.Close();
        }
    }
}
