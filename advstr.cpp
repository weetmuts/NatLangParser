/* Copyright (C) Fredrik Öhrström 1991 */

char intro_text[]=
"\n"
"   Syntax analysator 	av Fredrik Öhrström\n"
"   ----------------- \n"
"   Denna fil är ett frukstansvärt hack. Logiken skrevs på en halvtimme,\n"
"   efter att jag kommit på hur den skulle skrivas.\n"
"   Den använder sig av en (mini) generativ grammatik a la Chomsky\n"
"    följande regler finns:\n"
" 	S  = NP VP                               S=sats\n"
" 	NP = (det) (A) noun                      NP=nominalfras\n"
" 	A  = adj (A)				 VP=verbalfras\n"
" 	VP = verb [ PF || [ (NP) (PP) ] ]	 det=formord=a an the\n"
" 	PF = [ adj || NP ] 			 PF=predikatsfyllnad\n"
" 	PP = prep NP				 PP=prepositionsfras\n"
"    ord som finns:\n"
"	det = a an the  adj = red blue green good evil beautiful dark\n"
"	noun= man woman ball computer rock house forest\n"
"	verb= is gives takes looks sings goes stands runs\n"
"	prep= to at from in into inside on beside\n"
"\n"
"    Ex på mening: The dark woman gives a red ball to the man\n"
"		   An evil man goes into the house.\n\x0";
int indent;

#include <iostream>
#include <string>
#include <cstring>
#include <malloc.h>
#include <stdio.h>

#define	TRUE	-1
#define FALSE	0

const	char delimiters[8]={' ', '.', ',', '!', '?', 0 };


class 	STR {

protected:
  char nfo;
  unsigned int size;
  char *text;

  int delimiter_yn(char in_char);
  char tidy_char(char in_char);

public:
  STR() { nfo=0; size=0; text=0; }
  STR(int newsize) { nfo=2; size=newsize; text=new char[size]; }
    STR(const char *newtext) { nfo=1; size=1+strlen(newtext); text=(char*)malloc(size); strcpy(text, newtext); }
  STR(STR &x);
  ~STR() { if (nfo==2) free(text); }

  STR operator=(STR newstr);
  int operator==(STR &compstr);
  STR operator+(STR addstr);
  STR operator+(char *addstr);

  char  *gettext() { return text; }
  void  settext(char *t) { if (nfo<2) text=t; }
  int   nofwords();
  void  word(int nr, STR *x);
    void	pm() { printf(" mem=%u (%s)\n", /*coreleft()*/4242, text );  }
  void  pm(int a, int b, const char *extra);
  void 	tidy_text();
  void  c() { for (int i=0; i<size; i++) text[i]=0; }

};

void STR::pm(int a, int b, const char *extra)
{
  STR comp(80);

  for (int i=0; i<indent; i++) std::cout << " ";
  printf("%s", extra);
  for (int i=a; i<b; i++)
  {
    word(i, &comp);
    printf(" %s", comp.gettext());
  }
  printf("\n");
}


STR::STR(STR &x)
{
  nfo=2;
  size=x.size;
  text=new char[size];
  strcpy(text, x.text);
}

int STR::operator==(STR &compstr)
{
  return !strcmp(text, compstr.text);
}

STR STR::operator=(STR newstr)
{
    if (nfo!=0 && size>=newstr.size) strcpy(text, newstr.text);
    return *this;
}

STR STR::operator+(STR addstr)
{
  STR temp(size+addstr.size-1);
  strcpy(temp.text, text);
  strcat(temp.text, addstr.text);
  return temp;
}

STR STR::operator+(char *addstr)
{
  STR temp(size+strlen(addstr));
  strcpy(temp.text, text);
  strcat(temp.text, addstr);
  return temp;
}

int STR::delimiter_yn(char in_char)
{
  if ( strchr( delimiters, in_char ) == NULL ) return FALSE;
  else					       return TRUE;
}

char STR::tidy_char(char in_char) {

  if ( (in_char>='a' && in_char<='z') ||
       (in_char>='0' && in_char<='9') ||
      delimiter_yn(in_char) )			return in_char;
  else
  if (  in_char>='A' && in_char<='Z'  )		return in_char+('a'-'A');
  else
  switch (in_char) {
    case 39  :
    case 96  : return '"';
    default  : return ' ';
  }
}

void 	STR::tidy_text()
{
	int in_pos, out_pos;

	for (in_pos=out_pos=0; text[in_pos]!=0; in_pos++, out_pos++) {
	  text[in_pos]=tidy_char(text[in_pos]);
	  if (delimiter_yn(text[in_pos])) {
	    if (out_pos==0) out_pos--;
	    else
	    if (delimiter_yn(text[out_pos-1])) {
	      if (text[out_pos-1]!=' ') {
		out_pos--;
	      } else {
		out_pos--;
		text[out_pos]=text[in_pos];
	      }
	    } else
	    text[out_pos]=text[in_pos];
	  } else
	  text[out_pos]=text[in_pos];
	}
	text[out_pos]=0;
}

int STR::nofwords()
{
  int wordcount, length, wordpos;

  length=strlen(text);
  for (wordcount=wordpos=0; wordpos<length; wordcount++)
  {
    wordpos+=strcspn((text+wordpos), delimiters);
    wordpos++;
  }
  return wordcount;
}

void STR::word(int nr, STR *dest)
{
  int wordcount, wordpos, wordendpos;

  nr++;
  for (wordcount=wordpos=wordendpos=0; wordcount<nr; wordcount++)
  {
    wordpos=wordendpos;
    wordendpos+=strcspn((text+wordendpos), delimiters);
    wordendpos++;
  }
  wordendpos--;
  memcpy(dest->text, (text+wordpos), wordendpos-wordpos);
  dest->text[wordendpos-wordpos]=0;
}

int find(const char *l, STR *compare)
{
  STR comp(80), list(l);
  int i, nofwords;

  nofwords=list.nofwords();
  for (i=0; i<nofwords; i++)
  {
    list.word(i, &comp);
    if (*compare==comp) { return 1; }
  }
  return 0;
}

//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
// 		Här kommer logiken!!!
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================

int 	SQ(STR *text, int os);
int 	NPQ(STR *text, int os);
int 	VPQ(STR *text, int os);
int 	AQ(STR *text, int os);
int 	PFQ(STR *text, int os);
int 	PPQ(STR *text, int os);
int 	detQ(STR *text, int os);
int 	adjQ(STR *text, int os);
int 	nounQ(STR *text, int os);
int 	verbQ(STR *text, int os);
int 	prepQ(STR *text, int os);


int SQ(STR *text, int os)
{
  int s=os, add;

  indent+=4;
  if ( (add=NPQ(text, s)) )
  {
    s+=add;
    if ( (add=VPQ(text, s)) )
    {
      s+=add;
      text->pm(os, s, "S =");
      indent-=4;
      return os-s;
    }
  }
  indent-=4;
  return 0;
}

int VPQ(STR *text, int os)
{
  STR is("is");   // UjUj, vilka verb som kräver PF o.s.v kommer förstås att
  STR verbet(80);      // ligga i en ordlista sedan..
  int s=os, add;

  indent+=4;
  if ( (add=verbQ(text, s)) )
  {
    text->word(s, &verbet);  			 // plocka ut verbet
    s+=add;
    if ( (verbet==is) && (add=PFQ(text, s)) ) // här är det bara "is"
					      // som vill ha PF efter sig
    {
      s+=add;
      text->pm(os, s, "VP =");
      indent-=4;
      return s-os;
    }
    s+=NPQ(text, s);
    s+=PPQ(text, s);
    text->pm(os, s, "VP =");
    indent-=4;
    return s-os;
  }
  return -111111;
}

int NPQ(STR *text, int os)
{
  int s=os;

  indent+=4;
  s+=detQ(text, s);
  s+=AQ(text, s);
  if ( nounQ(text, s) )
  {
    s++;
    text->pm(os, s, "NP =");
    indent-=4;
    return s-os;
  }
  indent-=4;
  return 0;
}

int AQ(STR *text, int os)
{
  int add;

  indent+=4;
  if ( adjQ(text, os) )
  {
    add=AQ(text, os+1);
    text->pm(os, os+1+add, "A =");
    indent-=4;
    return 1+add;
  }
  indent-=4;
  return 0;
}

int PFQ(STR *text, int os)
{
  int add;

  indent+=4;
  if ( add=adjQ(text, os) )
    { text->pm(os, os+add, "PF ="); indent-=4; return add; }
  if ( add=NPQ(text, os) )
    { text->pm(os, os+add, "PF ="); indent-=4; return add; }
  indent-=4;
  return 0;
}

int PPQ(STR *text, int os)
{
  int s=os,add;

  indent+=4;
  if ( add=prepQ(text, s) )
  {
    s+=add;
    if ( add=NPQ(text, s) ) {
      s+=add;
      text->pm(os, s, "PP =");
      indent-=4;
      return s-os;
    }
  }
  indent-=4;
  return 0;
}



int adjQ(STR *text, int os)
{
  STR c(80);

  indent+=4;
  text->word(os, &c);
  if ( find("red blue green good evil beautiful dark", &c) )
  {
    text->pm(os, os+1, "adj =");
    indent-=4;
    return 1;
  }
  indent-=4;
  return 0;
}
int detQ(STR *text, int os)
{
  STR c(80);

  indent+=4;
  text->word(os, &c);
  if ( find("a an the", &c) )
  {
    text->pm(os, os+1, "det =");
    indent-=4;
    return 1;
  }
  indent-=4;
  return 0;
}
int verbQ(STR *text, int os)
{
  STR c(80);

  indent+=4;
  text->word(os, &c);
  if ( find("is gives takes looks sings goes stands runs", &c) )
  {
    text->pm(os, os+1, "verb =");
    indent-=4;
    return 1;
  }
  indent-=4;
  return 0;
}
int nounQ(STR *text, int os)
{
  STR c(80);

  indent+=4;
  text->word(os, &c);
  if ( find("man woman ball computer rock house forest cave", &c) )
  {
    text->pm(os, os+1, "noun =");
    indent-=4;
    return 1;
  }
  indent-=4;
  return 0;
}

int prepQ(STR *text, int os)
{
  STR c(80);

  indent+=4;
  text->word(os, &c);
  if ( find("to at from in into inside on beside", &c) )
  {
    text->pm(os, os+1, "prep =");
    indent-=4;
    return 1;
  }
  indent-=4;
  return 0;
}


int main() {
  STR sats;
  char text[256];

  puts( intro_text );
  std::cout << ">";

  std::cin.get(text, 250);
  puts(text);
  sats.settext(text);
  sats.tidy_text();
  sats.pm();

  indent=0;
  SQ(&sats, 0);
  return 0;
}
