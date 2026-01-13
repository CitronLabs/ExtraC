#include "../GXinternal.h"
#include "includes.h"
#include "unistd.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define PGEDIT_W 0x01
#define PGEDIT_H 0x02
#define PGEDIT_X 0x04
#define PGEDIT_Y 0x08

typedef struct infobox infobox;

typedef struct infobox{
	uint16_t _x,_y,_w,_h;
	wchar_t** _buffer;
	uint16_t* _bufferusage;
	infobox* _firstchild;
	infobox* _nextsibling;
	uint16_t linecursor;

}infobox;
void initScreen(){
	
	wprintf(L"\033[?1049h");
	wprintf(L"\033[?47h");

}
void quitScreen(){
	
	wprintf(L"\033[?1049l");


}
infobox* initPage(uint16_t w, uint16_t h){
	infobox* respage = calloc(sizeof(infobox),1);

	if(w < 1)
		respage->_h = 1;
	else
		respage->_h = h;
	if(w < 2)
		respage->_w = 2;
	else
		respage->_w = w;
	respage->_x = 0;
	respage->_y = 0;
	respage->linecursor = 0;

	respage->_bufferusage = calloc(sizeof(uint16_t), respage->_h);

	respage->_buffer = calloc(sizeof(wchar_t*), respage->_h);
	loop(i, respage->_h){
	respage->_buffer[i] = calloc(sizeof(wchar_t), respage->_w);
	}
	return respage;
}

void editPage(infobox* page, 
	      uint16_t w,uint16_t h,
	      uint16_t x,uint16_t y, 
	      uint8_t edits)
{
	getbitflagsinit
	getbitflags(edits,{
	case PGEDIT_X:{page->_x = x; break;}
	case PGEDIT_Y:{page->_y = y; break;}
	case PGEDIT_W:{page->_w = w; break;}
	case PGEDIT_H:{page->_h = h; break;}
	})
}
uint32_t writePage(infobox* page, 
	       uint16_t line,uint16_t offset,
	       const char* text, uint64_t len)
{

	wchar_t* linebuffer = page->_buffer[line];
	uint16_t lineinc = 0, tempcursor = 0;
	uint16_t* linecursor = NULL;
newline:

	linecursor = &(page->_bufferusage[line + lineinc]);
	loop(i, offset){
		linebuffer[i] = 0x20;
		(*linecursor)++;
	}

	while (len - tempcursor > 0) {
		linebuffer[*linecursor] = text[tempcursor];
		tempcursor++;
		(*linecursor)++;
		if(*linecursor > (page->_w-2)){
			page->_bufferusage[line + lineinc] += *linecursor;
			lineinc++;
			linebuffer = page->_buffer[line + lineinc];
			goto newline;
		}
	}
	linebuffer[*linecursor] = text[tempcursor];
return lineinc;
}


void echoPage(infobox* page){
	
	wprintf(L"X: %i Y: %i\n", page->_x, page->_y);
	wprintf(L"W: %i H: %i\n", page->_w, page->_h);
	wprintf(L"LINE CURSOR: %i\n", page->linecursor);
	
	wprintf(L"BUFFER USAGE %p: %i\n", page->_bufferusage, *page->_bufferusage);
	
	wprintf(L"INSIDE BUFFER %p:\n", page->_buffer);
	loop(i, page->_h)
	wprintf(L"\t %p: %ls\n", page->_buffer[i]);

	wprintf(L"FIRST CHILD %p:\n", page->_firstchild);
	wprintf(L"NEXT SIBLING %p:\n", page->_nextsibling);
}

#define isLetters(spec) \
	(spec >= 'A' && spec <= 'Z') || \
	(spec >= 'a' && spec <= 'z')
uint32_t writePageWide(infobox* page, 
	       uint16_t line,uint16_t offset,
	       const wchar_t* text, uint64_t len)
{
		

	wchar_t* linebuffer = page->_buffer[line];
	uint16_t lineinc = 0, tempcursor = 0, cutoffnum = 0;;
	uint16_t* linecursor = NULL;
	bool cutoff = false;
newline:
	
	linecursor = &(page->_bufferusage[line + lineinc]);
	loop(i, offset){
		linebuffer[i] = 0x20;
		(*linecursor)++;
	}
	if(cutoff){
	while (cutoffnum > 0) {
		linebuffer[*linecursor] = text[tempcursor - cutoffnum];
		(*linecursor)++;
		cutoffnum--;
	}
	cutoff = false;
	}

	while (len - tempcursor > 0) {
		linebuffer[*linecursor] = text[tempcursor];
		tempcursor++;
		(*linecursor)++;
		if(*linecursor > (page->_w-2)){
			while(isLetters(linebuffer[*linecursor - 1 - cutoffnum])){
			linebuffer[*linecursor - 1 - cutoffnum] = 0x20;
			cutoffnum++;
			cutoff = true;
			}

			page->_bufferusage[line + lineinc] += *linecursor;
			lineinc++;
			linebuffer = page->_buffer[line + lineinc];
			goto newline;
		}
	}
	(*linecursor)++;
	linebuffer[*linecursor] = text[tempcursor];
return lineinc;
}

void nestPage(infobox* parent, infobox* child, uint16_t line, uint16_t offset){

	infobox* current = parent->_firstchild;
	if(current != NULL){
		while (current->_nextsibling != NULL) {
			current = current->_nextsibling;	
		}
		current->_nextsibling = child;
	}else {
	parent->_firstchild = child;
	}
	child->_y = line + parent->_y;
	child->_x = offset + parent->_x;
}

typedef struct printContext{
	uint16_t parentoffset_x, parentoffset_y;
	uint16_t rootwidth;

}pcontext;

typedef struct printVars{
	uint16_t _x,_y,_w,_h;
	wchar_t** _buffer;
	uint16_t* _bufferusage;
	infobox* _firstchild;
	infobox* _nextsibling;
}pvars;
#define x vars->_x
#define y vars->_y
#define w vars->_w
#define h vars->_h
#define buffer vars->_buffer
#define bufferusage vars->_bufferusage
#define firstchild vars->_firstchild
#define nextsibling vars->_nextsibling


#define TOP_BORDER L"\u2581"
#define LEFT_BORDER L"\u258E"
#define RIGHT_BORDER L"\u2595"
#define BOTTOM_BORDER L"\u2594"

#define TOP_BORDER L" "
#define LEFT_BORDER L" "
#define RIGHT_BORDER L" "
#define BOTTOM_BORDER L" "

void printLine(wchar_t* out, pvars* vars,uint16_t line){
		

	loop(i, (w - bufferusage[line]) - 1)
		(buffer[line])[bufferusage[line] + i - 1] = 0x20;


		swprintf(out, w, L"%ls%ls", buffer[line], RIGHT_BORDER);
}
bool printRecursive(pvars* vars,pcontext* ctx, wchar_t* outbuffer){

	uint64_t cursor = 0;
	int cx = 0;


	

	//RENDERING THE TOP OF THE BOX
	if(w == ctx->rootwidth)
		cursor = ((y*w) + x);
	else
		cursor = ((y*(ctx->rootwidth + 1)) + x);
	loop(i, w)

		cx = swprintf(outbuffer + cursor + i, 100,TOP_BORDER);
		if(cx == -1) return false;
	if(w == ctx->rootwidth)
		cursor+=w;
	else{
	 	cursor+=(w + (ctx->rootwidth - (w + x)));
		outbuffer[cursor-(ctx->rootwidth - (w + x))] = 0x20;
		cursor++;
	}

	//LOOPING THROUGH THE WHOLE HEIGHT OF THE BOX AND RENDERING THE 
	//TEXT AND SIDES OF THE BOX
	loop(i, h){
		if(w == ctx->rootwidth){
			cx = swprintf(outbuffer + cursor, 100, L"\n");
		cursor++;
		}
		cursor+= x;
		cx = swprintf(outbuffer + cursor, 2, LEFT_BORDER);
		cursor++;
		
		printLine(outbuffer + cursor,vars,i);
		
		if(w == ctx->rootwidth)
			cursor+=w - 1;
		else{
		 	cursor+=w + (ctx->rootwidth - (w + x));
			outbuffer[cursor-(ctx->rootwidth - (w + x)) -1] = 0x20;
		}
	}
	if(w == ctx->rootwidth){
		swprintf(outbuffer + cursor , 100,L"\n");
		cursor++;
	}

	//FINALLY ONE LAST LOOP THROUGH FOR THE BOTTOM OF THE BOX
	loop(i, w)
		swprintf(outbuffer + cursor + i + x, 100,BOTTOM_BORDER);
	if(w == ctx->rootwidth)
		cursor+=w;
	else{
	 	cursor+=(w + (ctx->rootwidth - (w + x)));
		outbuffer[cursor-(ctx->rootwidth - (w + x)) + x] = 0x20;
	}

	//THEN WE RENDER ANY CHILDREN AND SIBLINGS
	if(firstchild != NULL){
		pvars nvars = {
		._w = firstchild->_w,
		._h = firstchild->_h,
		._x = firstchild->_x - ctx->parentoffset_x,
		._y = firstchild->_y - ctx->parentoffset_y,
		._buffer = firstchild->_buffer,
		._bufferusage = firstchild->_bufferusage,
		._nextsibling = firstchild->_nextsibling,
		._firstchild = firstchild->_firstchild,
		};
		printRecursive(&nvars, ctx ,outbuffer);
	}
	if(nextsibling != NULL){
		pvars nvars = {
		._w = nextsibling->_w,
		._h = nextsibling->_h,
		._x = nextsibling->_x - ctx->parentoffset_x,
		._y = nextsibling->_y - ctx->parentoffset_y,
		._buffer = nextsibling->_buffer,
		._bufferusage = nextsibling->_bufferusage,
		._firstchild = nextsibling->_firstchild,
		._nextsibling = nextsibling->_nextsibling,
		};
		printRecursive(&nvars, ctx ,outbuffer);
	}
	return true;
}

void printPage(infobox* page){
	
	wchar_t outbuffer[1000000];
	
	pvars vars = {
	._w = page->_w, ._h = page->_h,
	._x = 0, ._y = 0,
	._buffer = page->_buffer,
	._bufferusage = page->_bufferusage,
	._firstchild = page->_firstchild,
	._nextsibling = page->_nextsibling
	};
	pcontext context = {
	.parentoffset_x = page->_x, .parentoffset_y = page->_y,
	.rootwidth = page->_w
	};



	if(printRecursive(&vars, &context ,outbuffer)){
		fputws(outbuffer, stdout);
	}
	else
	 printf("An Error Has Occured when printing the page");
}

uint16_t centerpos(infobox* page){return page->_w/2;}


