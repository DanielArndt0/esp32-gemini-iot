#include "GeminiPrompts.h"

namespace GeminiPrompts
{

  String asciiOnlyPtBr(const String &userTopic, int maxLines)
  {
    String p;
    p += "Responda em portugues do Brasil, mas sem acentos e sem caracteres especiais.\n";
    p += "Use somente ASCII (a-z, 0-9, pontuacao basica).\n";
    p += "Nao use emojis.\n";
    p += "Maximo de ";
    p += String(maxLines);
    p += " linhas.\n\n";
    p += "Tema: ";
    p += userTopic;
    return p;
  }

}
