/* Game TIC TAC TOE
 * Author: ipri_c0der
 * Copyright (C) Oktober 2002 - Khayalan Production House
 * ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 *
 * Description:
 * 	Program ini dibuat dengan memperhitungkan posisi lawan. Jika posisi
 * lawan membahayakan atau hampir membentuk satu garis, maka computer akan
 * memposisikan dirinya pada titik penghubung tersebut. Game ini hanya berisi
 * satu level (dalam pengembangannya akan dicoba untuk beberapa level).
 *	Untuk interface, game ini hanya menggunakan layar hitam-putih atau
 * layar dos yang berukuran 80 * 25.
 *
 */

/* All Include */
#include "color.h"
#include <stdlib.h>


/* All Variable */
int win;
int move;
int turn;
int temp;
int quit;
int mute;
int check;
int x1,y1;
int picked[9];
int player1,player2;
int Phistory[9], Thistory[9];
int NPicked; // NPicked 1 = Human1, 2 = Human2, 3 = Comp1, 4 = Comp2
int nolevel; // nolevel 1 = Beginner, 2 = This is hard, 3 = Master from hell
int noPlayMode; // noPlayMode 1 = Human VS Comp, 2 = 2 Human, 3 = 2 Computer
int BoxX[9]={28,48,68,28,48,68,28,48,68};
int BoxY[9]={19,19,19,11,11,11,3,3,3};
int Oscore,Xscore, Owin,Xwin, total;
char *level; *PlayMode;
char images[6][6]=
		  {
		   " *** ",
		   "*   *",
		   "\\   /",
		   " \\ / ",
		   " / \\ ",
		   "/   \\"
		  };

/* Main Functions */
void Beep(int Hz, int ms)
{
 if (!mute)
 {
  sound(Hz);
  delay(ms);
  nosound();
 }
}

void Player1Move(void)
{
 Beep(250,100);
 Beep(260,300);
}

void Player2Move(void)
{
 Beep(260,100);
 Beep(250,300);
}

void Player1Win()
{
 delay(1000);
 Beep(180,300);
 Beep(220,300);
 Beep(200,300);
 Beep(250,300);
 Beep(220,300);
 Beep(260,300);
}

void Player2Win()
{
 delay(1000);
 Beep(170,1000);
 Beep(130,1500);
}

void Draw(void)
{
 delay(1000);
 Beep(250,500);
 Beep(250,500);
 Beep(260,1000);
}

void Clr(int color)
{
 gotoxy(1,25);
 textattr(color);
 clreol();
}

void PutStr(int x, int y, int color, char *s)
{
 gotoxy(x,y);
 textattr(color);
 cprintf(s);
}

void PutCtr(int x1, int x2, int y, int color, char *s)
{
 int x;

 x=x1 + ((((x2-x1)+1) - strlen(s))/2);
 PutStr(x,y,color,s);
}

void OpenStr(int y, char *s, int dly)
{
 int i=0,front,rear;

 do
 {
  gotoxy(41-i,y);
  for (front=0; front<=i; front++) cprintf("%c", s[front]);
  for (rear=strlen(s)-i+1; rear<=strlen(s)-1; rear++) cprintf("%c", s[rear]);
  Beep(200,dly*10);
  i++;
 } while (!(i==strlen(s)/2+1));
}

void ShowUp(int y, int tc, int bc, char *s, int dly, int tech)
{
 gotoxy(1,y);
 textcolor(bc);
 textbackground(bc);
 clreol();
 delay(dly*100);
 if (tech==1)
 {
  textattr(tc + (bc<<4));
  OpenStr(y, s, dly);
 }
 if (tech==2)
 {
  PutCtr(1,80, y, (tc-8) + (bc<<4), s);
  delay(dly*100);
 }
 PutCtr(1,80, y, tc + (bc<<4), s);
 delay(dly*300);
 PutCtr(1,80, y, (tc-8) + (bc<<4), s);
 delay(dly*100);
 PutCtr(1,80, y, bc + (bc<<4), s);
 delay(dly*100);
}

void Begin(void)
{
 int i;
 char *text[4];
 text[0]="Copyright (C) Oktober 2002 - Khayalan Production House";
 text[1]="created by \"ipri_c0der\"";
 text[2]="present";
 text[3]="TIC TAC TOE v1.6";

 // Perlihatkan tampilan pembuka
 textattr(_00);
 clrscr();
 PutCtr(1,80, 12, _09, text[0]);
 PutCtr(1,80, 14, _09, text[1]);
 delay(1000);
 for (i=1; i<=80; i++)
 {
  PutStr(i,13, _0F,"Ä");
  PutStr(81-i,13, _0C,"Ä");
  gotoxy(79,25);
  Beep(800-(i*10),10);
 }
 delay(1000);
 PutStr(67,12, _09,"ü");
 Beep(120,600);
 Beep(100,1000);
 PutStr(1,24, _0F,"STIKI MALANG - INDONESIA");
 PutStr(1,25, _0F,"University of computer technology");
 delay(2000);
 PutCtr(1,80, 12, _01, text[0]);
 PutStr(67,12, _01,"ü");
 for (i=1; i<=40; i++)
 {
  PutStr(i,13, _04,"Ä");
  PutStr(40+i,13, _07,"Ä");
 }
 PutCtr(1,80, 14, _01, text[1]);
 delay(2000);
 clrscr();
 ShowUp(13, 15, 0, text[2], 5, 2);
 ShowUp(13, 14, 4, text[3], 5, 1);

 // Game Background
 textattr(_00); clrscr();
 window(20,1,80,24);
 textattr(_90); clrscr();
 window(1,1,80,25);
 for (i=1; i<=80; i++)
 {
  PutStr(i+1,1,  _1F, "ß");
  PutStr(i+1,24, _1F, "Ü");
  if (i >= 20)
  {
   PutStr(i,9,  _1F, "ß");
   PutStr(i,17, _1F, "ß");
  }
  if ((i>=2) && (i<=19))
  {
   PutStr(i,3,  _0F, "Ä");
   PutStr(i,7,  _0F, "Ä");
   PutStr(i,11, _0F, "Ä");
   PutStr(i,5,  _1F, "ß");
   PutStr(i,9,  _1F, "ß");
   PutStr(i,13, _1F, "Û");
   PutStr(i,18, _1F, "Û");
   PutStr(i,21, _1F, "Û");
   PutStr(i,22, _1F, "ß");
  }
 }
 for (i=1; i<=24; i++)
 {
  PutStr(1,i,  _1F, "Û");
  PutStr(80,i, _1F, "Û");
  PutStr(20,i, _1F, "Û");
  PutStr(40,i, _1F, "Û");
  PutStr(60,i, _1F, "Û");
 }
 PutCtr(1,20,6,  _04, "Level");
 PutStr(2,2,  _04, "O Score  X Score");
 PutStr(10,2, _0F,"³");
 PutStr(10,3, _0F,"Å");
 PutStr(10,4, _0F,"³");
 PutStr(2,10, _04, "O Win X Win Total");
 PutStr(7,10, _0F,"³");
 PutStr(7,11, _0F,"Å");
 PutStr(7,12, _0F,"³");
 PutStr(13,10, _0F,"³");
 PutStr(13,11, _0F,"Å");
 PutStr(13,12, _0F,"³");

 PutStr(3,14, _8F, "Tip:");
 PutStr(3,15, _07, "Gunakan keypad");
 PutStr(3,16, _07, "untuk kenyamanan");
 PutStr(3,17, _07, "dalam bermain.");
 PutStr(3,19, _07, "[`] Options");
 PutStr(3,20, _07, "[~] About me!");
 PutStr(3,23, _09, text[3]);
}

void End(void)
{
 int i;

 // Clear Boxes
 for (i=1; i<=12; i++)
 {
  window(1,25-i,80,25); delay(20); clrscr();
  window(1,i,80,i); delay(20); clrscr();
  window(1,1,80,25);
 }

 // Say Goodbye. . .
 textattr(_00);
 clrscr();
 delay(300);
 ShowUp(12, 15, 0,"...Thank's for playing...", 5, 2);
 textattr(_07);
 clrscr();
}

void ResetPoint(void)
{
 Owin=0;
 Xwin=0;
 Oscore=0;
 Xscore=0;
}

void ShowPoint(void)
{
 // Clear Positions
 PutStr(3,8,   _07,"                ");
 PutStr(2,4,   _07,"        ");
 PutStr(11,4,  _07,"         ");
 PutStr(2,12,  _07,"     ");
 PutStr(8,12,  _07,"     ");
 PutStr(14,12, _07,"      ");

 // Level label
 if (nolevel==1) level="Beginner";
 if (nolevel==2) level="This is hard";
 if (nolevel==3) level="Master from hell";
 if (noPlayMode==2) level="Human VS Human";
 if (noPlayMode==3) level="Comp VS Comp";
 gotoxy(14,6);
 if (noPlayMode==2 || noPlayMode==3) cprintf("%i", nolevel);
 else cprintf(" ");


 // Show the point
 PutCtr(1,20,8, _07, level);
 gotoxy(2,12);  if (Owin<=30000)       			    printf("%d", Owin);   else printf("MAX");
 gotoxy(8,12);  if (Xwin<=30000)       			    printf("%d", Xwin);   else printf("MAX");
 gotoxy(14,12); if (total<=30000)     			    printf("%d", total);  else printf("MAX");

 // for scorring
 gotoxy(2,4); if (Oscore>=-30000 && Oscore<=30000)
 {
  if (Oscore!=0) printf("%d00", Oscore);
  else printf("%d", Oscore);
 }
 else if (Oscore<=30000) printf("MAXIMUM");
 else if (Oscore>=-30000) printf("MINIMUM");
 gotoxy(11,4); if (Xscore>=-30000 && Xscore<=30000)
 {
  if (Xscore!=0) printf("%d00", Xscore);
  else printf("%d", Xscore);
 }
 else if (Xscore<=30000) printf("MAXIMUM");
 else if (Xscore>=-30000) printf("MINIMUM");
}

void ReloadBackground(void)
{
 int i;

 // Box background
 textbackground(1);
 window(22,2,38,7); clrscr();
 window(42,2,58,7); clrscr();
 window(62,2,78,7); clrscr();
 window(22,10,38,15); clrscr();
 window(42,10,58,15); clrscr();
 window(62,10,78,15); clrscr();
 window(22,18,38,23); clrscr();
 window(42,18,58,23); clrscr();
 window(62,18,78,23); clrscr();
 window(1,1,80,25);

 // Write digit into Boxes
 PutCtr(20,40,21, _1F,"1");
 PutCtr(40,60,21, _1F,"2");
 PutCtr(60,80,21, _1F,"3");
 PutCtr(20,40,13, _1F,"4");
 PutCtr(40,60,13, _1F,"5");
 PutCtr(60,80,13, _1F,"6");
 PutCtr(20,40,5,  _1F,"7");
 PutCtr(40,60,5,  _1F,"8");
 PutCtr(60,80,5,  _1F,"9");

 // Clear picked point
 for (i=0; i<=8; i++) picked[i]=0;
}

void DrawO(int color)
{
 x1=BoxX[NPicked];
 y1=BoxY[NPicked];

 PutStr(x1,y1,   color, images[0]);
 PutStr(x1,y1+1, color, images[1]);
 PutStr(x1,y1+2, color, images[1]);
 PutStr(x1,y1+3, color, images[0]);
}

void DrawX(int color)
{
 x1=BoxX[NPicked];
 y1=BoxY[NPicked];

 PutStr(x1,y1  , color, images[2]);
 PutStr(x1,y1+1, color, images[3]);
 PutStr(x1,y1+2, color, images[4]);
 PutStr(x1,y1+3, color, images[5]);
}

void Option(void)
{
 int check1,check2;
 int TLevel,TPlayMode;

 check=0;
 TLevel=nolevel;
 TPlayMode=noPlayMode;
 do
 {
  Clr(_07);
  PutStr(1,25 ,_07, "<1> Turn play mode ³ <2> Turn level ³ <3> Turn pc speaker ³ Anykey to play ³> ");
  check1=getch()-48;
  Clr(_07);
  if (check1==1)
  {
   PutStr(1,25, _07, "Turn play mode [1/2/3]: ");
   check2=getch()-48;
   if (check2>=1 && check2<=3)
   if (noPlayMode!=check2)
   {
    check=0;
    noPlayMode=check2;
   }
   if (noPlayMode==1) ShowUp(25, 14, 4, "Play mode turn to \"Human Vs Computer\"", 3, 2);
   if (noPlayMode==2) ShowUp(25, 14, 4, "Play mode turn to \"Human Vs Human\"", 3, 2);
   if (noPlayMode==3) ShowUp(25, 14, 4, "Play mode turn to \"Computer Vs Computer\"", 3, 2);
  }
  if (check1==2)
  {
   PutStr(1,25, _07, "Turn Level [1/2/3]: ");
   check2=getch()-48;
   if (check2>=1 && check2<=3)
   if (nolevel!=check2)
   {
    check=0;
    nolevel=check2;
   }
   if (nolevel==1) ShowUp(25, 14, 4, "Level turn to \"Beginner\"", 3, 2);
   if (nolevel==2) ShowUp(25, 14, 4, "Level turn to \"This is hard\"", 3, 2);
   if (nolevel==3) ShowUp(25, 14, 4, "Level turn to \"Master from hell\"", 3, 2);
  }
  if (check1==3)
  {
   check=1;
   PutStr(1,25, _07, "Turn pc speaker [1=on, 2=off]: ");
   check2=getch()-48;
   if (check2>=1 && check2<=2) mute=check2-1;
   if (!mute) ShowUp(25, 14, 4, "PC speaker activated", 3, 2);
   if (mute) ShowUp(25, 14, 4, "PC speaker de-activated", 3, 2);
  }
  ShowPoint();
  check=(TLevel==nolevel && TPlayMode==noPlayMode)?1:0;
 } while ((check1>=1 && check1<=3));
}

void AboutMe(void)
{
 ShowUp(25, 15, 1, "The project from \"Khayalan Produciton House\"", 6, 2);
 ShowUp(25, 10, 1, "ipri_c0der make this game",3, 2);
 ShowUp(25, 15, 1, "from traditional game called", 4, 2);
 ShowUp(25, 15, 4, "-=([ TIC TAC TOE ])=-", 5, 1);
}

int CheckWin(void)
{
 int i=1;

 do
 {
  // Check Vertical
  if (picked[6]==i && picked[3]==i && picked[0]==i)
  {
   win = i;
   NPicked=6; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=3; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=0; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   break;
  }
  if (picked[7]==i && picked[4]==i && picked[1]==i)
  {
   win = i;
   NPicked=7; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=4; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=1; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   break;
  }
  if (picked[8]==i && picked[5]==i && picked[2]==i)
  {
   win = i;
   NPicked=8; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=5; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=2; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   break;
  }

  // Check Horizontal
  if (picked[6]==i && picked[7]==i && picked[8]==i)
  {
   win = i;
   NPicked=6; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=7; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=8; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   break;
  }
  if (picked[3]==i && picked[4]==i && picked[5]==i)
  {
   win = i;
   NPicked=3; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=4; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=5; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   break;
  }
  if (picked[0]==i && picked[1]==i && picked[2]==i)
  {
   win = i;
   NPicked=0; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=1; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=2; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   break;
  }

  // Check Diagonal
  if (picked[6]==i && picked[4]==i && picked[2]==i)
  {
   win = i;
   NPicked=6; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=4; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=2; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   break;
  }
  if (picked[8]==i && picked[4]==i && picked[0]==i)
  {
   win = i;
   NPicked=8; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=4; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   NPicked=0; if (player1==turn) DrawX(_9A); else DrawO(_9A);
   break;
  }
  i++;
 } while (i<=4);
 return win;
}

void ComputerMove(int comp, int player)
{
 char ch;
 int i, empty=0;

 if (noPlayMode==3 && kbhit())
 {
  Clr(_11);
  PutStr(1,25, _1F,"Are you sure want to abort this game [Y/N]?: ");
  do
  {
   ch=toupper(getch());
   if (!(ch=='Y' || ch=='N')) Beep(800,10);
  } while (!(ch=='Y' || ch=='N'));
  if (ch=='Y') quit=1;
 }

 if (nolevel==1)
 {
  for (i=0; i<=8; i++) if (picked[i]<1) empty=1;
  if (empty)
  {
   do
   {
    NPicked=random(9);
   } while (picked[NPicked]>0);
  }
 }

 if (nolevel==2)
 {
  NPicked=9;

  // Check 7 Vertical
  if (picked[3]==player && picked[0]==player && picked[6]<1) NPicked=6;
  if (picked[6]==player && picked[0]==player && picked[3]<1) NPicked=3;
  if (picked[6]==player && picked[3]==player && picked[0]<1) NPicked=0;

  // Check 8 Vertical
  if (picked[4]==player && picked[1]==player && picked[7]<1) NPicked=7;
  if (picked[7]==player && picked[1]==player && picked[4]<1) NPicked=4;
  if (picked[7]==player && picked[4]==player && picked[1]<1) NPicked=1;

  // Check 9 Vertical
  if (picked[5]==player && picked[2]==player && picked[8]<1) NPicked=8;
  if (picked[8]==player && picked[2]==player && picked[5]<1) NPicked=5;
  if (picked[8]==player && picked[5]==player && picked[2]<1) NPicked=2;

  // Check 7 Horizontal
  if (picked[7]==player && picked[8]==player && picked[6]<1) NPicked=6;
  if (picked[6]==player && picked[8]==player && picked[7]<1) NPicked=7;
  if (picked[6]==player && picked[7]==player && picked[8]<1) NPicked=8;

  // Check 4 Horizontal
  if (picked[4]==player && picked[5]==player && picked[3]<1) NPicked=3;
  if (picked[3]==player && picked[5]==player && picked[4]<1) NPicked=4;
  if (picked[3]==player && picked[4]==player && picked[5]<1) NPicked=5;

  // Check 1 Horizontal
  if (picked[1]==player && picked[2]==player && picked[0]<1) NPicked=0;
  if (picked[0]==player && picked[2]==player && picked[1]<1) NPicked=1;
  if (picked[0]==player && picked[1]==player && picked[2]<1) NPicked=2;

  // Check 7 Diagonal
  if (picked[4]==player && picked[2]==player && picked[6]<1) NPicked=6;
  if (picked[6]==player && picked[2]==player && picked[4]<1) NPicked=4;
  if (picked[6]==player && picked[4]==player && picked[2]<1) NPicked=2;

  // Check 9 Diagonal
  if (picked[4]==player && picked[0]==player && picked[8]<1) NPicked=8;
  if (picked[8]==player && picked[0]==player && picked[4]<1) NPicked=4;
  if (picked[8]==player && picked[4]==player && picked[0]<1) NPicked=0;

  // Default action
  if (NPicked==9)
  {
   for (i=0; i<=8; i++) if (picked[i]<1) empty=1;
   if (empty)
   {
    do
    {
     NPicked=random(9);
    } while (picked[NPicked]>0);
   }
  }
 }

 if (nolevel==3)
 {
  NPicked=9;

  // Check player move
  // Check 7 Vertical
  if (picked[3]==player && picked[0]==player && picked[6]<1) NPicked=6;
  if (picked[6]==player && picked[0]==player && picked[3]<1) NPicked=3;
  if (picked[6]==player && picked[3]==player && picked[0]<1) NPicked=0;

  // Check 8 Vertical
  if (picked[4]==player && picked[1]==player && picked[7]<1) NPicked=7;
  if (picked[7]==player && picked[1]==player && picked[4]<1) NPicked=4;
  if (picked[7]==player && picked[4]==player && picked[1]<1) NPicked=1;

  // Check 9 Vertical
  if (picked[5]==player && picked[2]==player && picked[8]<1) NPicked=8;
  if (picked[8]==player && picked[2]==player && picked[5]<1) NPicked=5;
  if (picked[8]==player && picked[5]==player && picked[2]<1) NPicked=2;

  // Check 7 Horizontal
  if (picked[7]==player && picked[8]==player && picked[6]<1) NPicked=6;
  if (picked[6]==player && picked[8]==player && picked[7]<1) NPicked=7;
  if (picked[6]==player && picked[7]==player && picked[8]<1) NPicked=8;

  // Check 4 Horizontal
  if (picked[4]==player && picked[5]==player && picked[3]<1) NPicked=3;
  if (picked[3]==player && picked[5]==player && picked[4]<1) NPicked=4;
  if (picked[3]==player && picked[4]==player && picked[5]<1) NPicked=5;

  // Check 1 Horizontal
  if (picked[1]==player && picked[2]==player && picked[0]<1) NPicked=0;
  if (picked[0]==player && picked[2]==player && picked[1]<1) NPicked=1;
  if (picked[0]==player && picked[1]==player && picked[2]<1) NPicked=2;

  // Check 7 Diagonal
  if (picked[4]==player && picked[2]==player && picked[6]<1) NPicked=6;
  if (picked[6]==player && picked[2]==player && picked[4]<1) NPicked=4;
  if (picked[6]==player && picked[4]==player && picked[2]<1) NPicked=2;

  // Check 9 Diagonal
  if (picked[4]==player && picked[0]==player && picked[8]<1) NPicked=8;
  if (picked[8]==player && picked[0]==player && picked[4]<1) NPicked=4;
  if (picked[8]==player && picked[4]==player && picked[0]<1) NPicked=0;

  // Check computer move
  // Check 7 Vertical
  if (picked[3]==comp && picked[0]==comp && picked[6]<1) NPicked=6;
  if (picked[6]==comp && picked[0]==comp && picked[3]<1) NPicked=3;
  if (picked[6]==comp && picked[3]==comp && picked[0]<1) NPicked=0;

  // Check 8 Vertical
  if (picked[4]==comp && picked[1]==comp && picked[7]<1) NPicked=7;
  if (picked[7]==comp && picked[1]==comp && picked[4]<1) NPicked=4;
  if (picked[7]==comp && picked[4]==comp && picked[1]<1) NPicked=1;

  // Check 9 Vertical
  if (picked[5]==comp && picked[2]==comp && picked[8]<1) NPicked=8;
  if (picked[8]==comp && picked[2]==comp && picked[5]<1) NPicked=5;
  if (picked[8]==comp && picked[5]==comp && picked[2]<1) NPicked=2;

  // Check 7 Horizontal
  if (picked[7]==comp && picked[8]==comp && picked[6]<1) NPicked=6;
  if (picked[6]==comp && picked[8]==comp && picked[7]<1) NPicked=7;
  if (picked[6]==comp && picked[7]==comp && picked[8]<1) NPicked=8;

  // Check 4 Horizontal
  if (picked[4]==comp && picked[5]==comp && picked[3]<1) NPicked=3;
  if (picked[3]==comp && picked[5]==comp && picked[4]<1) NPicked=4;
  if (picked[3]==comp && picked[4]==comp && picked[5]<1) NPicked=5;

  // Check 1 Horizontal
  if (picked[1]==comp && picked[2]==comp && picked[0]<1) NPicked=0;
  if (picked[0]==comp && picked[2]==comp && picked[1]<1) NPicked=1;
  if (picked[0]==comp && picked[1]==comp && picked[2]<1) NPicked=2;

  // Check 7 Diagonal
  if (picked[4]==comp && picked[2]==comp && picked[6]<1) NPicked=6;
  if (picked[6]==comp && picked[2]==comp && picked[4]<1) NPicked=4;
  if (picked[6]==comp && picked[4]==comp && picked[2]<1) NPicked=2;

  // Check 9 Diagonal
  if (picked[4]==comp && picked[0]==comp && picked[8]<1) NPicked=8;
  if (picked[8]==comp && picked[0]==comp && picked[4]<1) NPicked=4;
  if (picked[8]==comp && picked[4]==comp && picked[0]<1) NPicked=0;

  // Default action
  if (NPicked==9)
  {
   for (i=0; i<=8; i++) if (picked[i]<1) empty=1;
   if (empty)
   {
    do
    {
     NPicked=random(9);
    } while (picked[NPicked]>0);
   }
  }
 }

 // Save and draw X or O if not quit
 if (!quit)
 if (picked[NPicked]<1)
 {
  move++;
  Phistory[move-1]=NPicked;
  picked[NPicked]=comp;
  if (comp==player2)
  {
   Player2Move();
   DrawX(_1A);
  } else
  {
   Player1Move();
   DrawO(_1A);
  }
 }
}

void YourMove(int player)
{
 char ch;
 int i, empty=0, first=1;

 do
 {
  for (i=0; i<=8; i++) if (picked[i]<1) empty=1;
  if (empty)
  {
   if (first) NPicked=getch()-49;
   if (NPicked==126-49)
   {
    AboutMe();
    Clr(_11);
    if (player==player1) PutStr(1,25, _1F,"Player O turn. . .");
    else PutStr(1,25, _1F,"Player X turn. . .");
   }
   if (NPicked==96-49)
   {
    Option();
    if (check)
    {
     Clr(_11);
     if (player==player1) PutStr(1,25, _1F,"Player O turn. . .");
     else PutStr(1,25, _1F,"Player X turn. . .");
    }
    else
    {
     quit=2;
     break;
    }
   }
   if (NPicked==27-49)
   {
    Clr(_11);
    PutStr(1,25, _1F,"Are you sure want to abort this game [Y/N]?: ");
    do
    {
     ch=toupper(getch());
     if (!(ch=='Y' || ch=='N')) Beep(800,10);
    } while (!(ch=='Y' || ch=='N'));
    if (ch=='Y')
    {
     Clr(_07);
     quit=1;
     break;
    }
    if (ch=='N')
    {
     Clr(_11);
     if (player==player1) PutStr(1,25, _1F,"Player O turn. . .");
     else PutStr(1,25, _1F,"Player X turn. . .");
    }
   }
   if (NPicked==8-49)
   {
    if (!(move<1)) move--;
    turn=Thistory[move];
    NPicked=Phistory[move];
    picked[NPicked]=0;
    if (player1==turn) DrawX(_11); else DrawO(_11);
    if (NPicked==0) PutCtr(20,40,21, _14,"1");
    if (NPicked==1) PutCtr(40,60,21, _14,"2");
    if (NPicked==2) PutCtr(60,80,21, _14,"3");
    if (NPicked==3) PutCtr(20,40,13, _14,"4");
    if (NPicked==4) PutCtr(40,60,13, _14,"5");
    if (NPicked==5) PutCtr(60,80,13, _14,"6");
    if (NPicked==6) PutCtr(20,40,5,  _14,"7");
    if (NPicked==7) PutCtr(40,60,5,  _14,"8");
    if (NPicked==8) PutCtr(60,80,5,  _14,"9");
    if (move>0)
    {
     PutStr(1,25, _1F, "Player ");
     textattr(_14);
     if (player1==turn) cprintf("X"); else cprintf("O");
     textattr(_1F);
     cprintf(" move has been deleted!");
     turn=player;
     Beep(800,300);
     Beep(800,300);
     delay(400);
     if (mute) delay(600);
    }
    else turn=temp;
    if (first)
    {
     first=0;
     NPicked=8-49;
    } else
    {
     NPicked=-1;
     break;
    }
   }
  } else break;
 } while (!(NPicked>=0 && NPicked<=8 && picked[NPicked]<1));

 if (!quit)
 if (picked[NPicked]<1)
 {
  move++;
  Phistory[move-1]=NPicked;
  picked[NPicked]=player;
  if (player==player1)
  {
   Player1Move();
   DrawO(_1A);
  } else
  {
   Player2Move();
   DrawX(_1A);
  }
 }
}

void scores(int Check)
{
 if (Check==1)
 {
  if (noPlayMode==2)
  {
   if (Oscore<=30000) if (move<3) Oscore+=5; else Oscore+=2;
   if (Xscore>=-30000) if (move<3) Xscore-=3; else Xscore-=1;
  } else
  {
   if (nolevel==1)
   {
    if (Oscore<=30000) if (move<3) Oscore+=4; else Oscore+=2;
    if (Xscore>=-30000) if (move<3) Xscore-=3; else Xscore-=1;
   }
   if (nolevel==2)
   {
    if (Oscore<=30000) if (move<3) Oscore+=6; else Oscore+=3;
    if (Xscore>=-30000) if (move<3) Xscore-=4; else Xscore-=2;
   }
   if (nolevel==3)
   {
    if (Oscore<=30000) if (move<3) Oscore+=8; else Oscore+=4;
    if (Xscore>=-30000) if (move<3) Xscore-=5; else Xscore-=3;
   }
  }
 }
 if (Check==2)
 {
  if (noPlayMode==2)
  {
   if (Xscore<=30000) if (move<3) Xscore+=5; else Xscore+=2;
   if (Oscore>=-30000) if (move<3) Oscore-=3; else Oscore-=1;
  }
  else
  {
   if (nolevel==1)
   {
    if (Xscore<=30000) if (move<3) Xscore+=4; else Xscore+=2;
    if (Oscore>=-30000) if (move<3) Oscore-=3; else Oscore-=1;
   }
   if (nolevel==2)
   {
    if (Xscore<=30000) if (move<3) Xscore+=6; else Xscore+=3;
    if (Oscore>=-30000) if (move<3) Oscore-=4; else Oscore-=2;
   }
   if (nolevel==3)
   {
    if (Xscore<=30000) if (move<3) Xscore+=8; else Xscore+=4;
    if (Oscore>=-30000) if (move<3) Oscore-=5; else Oscore-=3;
   }
  }
 }
}

void PrintLabel(void)
{
 if (win) Clr(_11);
 if (noPlayMode==1)
 {
  if (win<1)
  {
   if (turn==1)
   {
    ShowUp(25, 15, 1, "...It's your turn now...", 2, 1);
    PutStr(1,25, _1F, "Player O turn. . .");
   }
   if (turn==3) ShowUp(25, 15, 1, "...The computer thing what he can do...", 2, 1);
  }
  else if (win==1)
  {
   if (Owin<=30000) Owin++;
   scores(1);
   Player1Win();
   ShowUp(25, 14, 4, "I don't know what to say..., YOU WIN!", 5, 2);
  }
  else if (win==3)
  {
   if (Xwin<=30000) Xwin++;
   scores(2);
   Player2Win();
   ShowUp(25, 14, 4, "Sorry pal! You loooozzee the match!!", 5, 2);
  }
 }
 else if (noPlayMode==2)
 {
  if (win<1)
  {
   if (turn==1)
   {
    ShowUp(25, 15, 1, "First player turn now", 2, 1);
    PutStr(1,25, _1F, "Player O turn. . .");
   }
   if (turn==2)
   {
    ShowUp(25, 15, 1, "Second player turn now", 2, 1);
    PutStr(1,25, _1F, "Player X turn. . .");
   }
  }
  else if (win==1)
  {
   if (Owin<=30000) Owin++;
   scores(1);
   Player1Win();
   ShowUp(25, 14, 4, "First palyer win", 5, 2);
  }
  else if (win==2)
  {
   if (Xwin<=30000) Xwin++;
   scores(2);
   Player2Win();
   ShowUp(25, 14, 4, "Second player win", 5, 2);
  }
 }
 else if (noPlayMode==3)
 {
  if (win<1)
  {
   if (turn==3) ShowUp(25, 15, 1, "First computer turn now", 2, 1);
   if (turn==4) ShowUp(25, 15, 1, "Second computer turn now", 2, 1);
  }
  else if (win==3)
  {
   if (Owin<=30000) Owin++;
   scores(1);
   Player1Win();
   ShowUp(25, 14, 4, "First computer win", 5, 2);
  }
  else if (win==4)
  {
   if (Xwin<=30000) Xwin++;
   scores(2);
   Player2Win();
   ShowUp(25, 14, 4, "Second computer win", 5, 2);
  }
 }
}

void turns(int check1, int check2)
{
 int i, empty=0;

 for (i=0; i<=8; i++) if (picked[i]<1) empty=1;
 if (empty)
 {
  if (check1==check2 && !win)
  {
   PrintLabel();
   if (noPlayMode==1) if (player1==1) turn=3; else turn=1;
   if (noPlayMode==2) if (player1==1) turn=2; else turn=1;
   if (noPlayMode==3) if (player1==3) turn=4; else turn=3;
   if (noPlayMode==1 || noPlayMode==2) YourMove(player1);
   if (noPlayMode==3) ComputerMove(player1, turn);
   win=CheckWin();
   Thistory[move-1]=turn;
  } else
  {
   if (!win)
   {
    PrintLabel();
    if (noPlayMode==1) if (player2==3) turn=1; else turn=3;
    if (noPlayMode==2) if (player2==2) turn=1; else turn=2;
    if (noPlayMode==3) if (player2==4) turn=3; else turn=4;
    if (noPlayMode==1 || noPlayMode==3) ComputerMove(player2, turn);
    if (noPlayMode==2) YourMove(player2);
    win=CheckWin();
    Thistory[move-1]=turn;
   }
  }
 }
}

void Playing(void)
{
 // PlayMode label
 if (noPlayMode==1)
 {
  player1=1;
  player2=3;
  PlayMode="Human VS Computer";
 }
 if (noPlayMode==2)
 {
  player1=1;
  player2=2;
  PlayMode="Human VS Human";
 }
 if (noPlayMode==3)
 {
  player1=3;
  player2=4;
  PlayMode="Computer VS Computer";
 }

 // Print PlayMode to monitor
 ShowUp(25, 15, 2, PlayMode, 3, 2);

 // Starting
 if (temp==player2) turn=player1; else turn=player2;
 temp=turn;
 do
 {
  //Play the game!
  turns(player1, turn);
  if (win) break;
  if (quit) break;
 } while (!(move==9));

 // Check quit
 if (!quit)
 {
  // Check level
  if (win)
  {
   if (noPlayMode==1)
   {
    if (player2==win && nolevel>1) nolevel--;
    if (player1==win && nolevel<3) nolevel++;
   }
   PrintLabel();
   ShowPoint();
  }
  else
  {
   Clr(_11);
   Draw();
   ShowUp(25, 15, 5, "This game must be really really hard aren't you?? DRAW GAME!", 5, 2);
  }
 }
}


/* MAIN PROGRAM */
void main(void)
{
 char ch;
 int key, out, first=1;

 // Very first run
 Begin();
 randomize();
 ResetPoint();
 ShowPoint();
 do
 {
  // Reset point
  win=0;
  move=0;

  if (first)
  {
   out=0;
   first=0;
   nolevel=1;
   noPlayMode=1;
   ReloadBackground();
   ResetPoint();
   ShowPoint();
   Clr(_07);
   PutStr(1,25, 7,"<1> Human VS Comp ³ <2> Human VS Human ³ <3> Comp VS Comp ³ [Esc] to Quit ³> ");
   do
   {
    key=getch()-48;
    if (!((key>=1 && key<=3) || key==27-48 || key==96-48 || key==126-48)) Beep(800,10);
   } while (!((key>=1 && key<=3) || key==27-48 || key==96-48 || key==126-48));
   if (key==27-48)
   {
    Clr(_07);
    PutStr(1,25, 7,"Are you sure want to quit [Y/N]?: ");
    do
    {
     ch=toupper(getch());
     if (!(ch=='Y' || ch=='N')) Beep(800,10);
    } while (!(ch=='Y' || ch=='N'));
    if (ch=='Y') out=1;
    else
    {
     out=2;
     first=1;
    }
   }
  }
  if (key==126-48 || out==126-48) AboutMe();
  if (key==96-48)
  {
   Option();
   if (check)
   {
    out=0;
    key=0;
   }
   else
   {
    key=noPlayMode;
    ReloadBackground();
    ResetPoint();
    ShowPoint();
   }
  }
  if (key==126-48)
  {
   out=2;
   first=1;
  }
  if (out==126-48)
  {
   out=0;
   key=0;
  }
  if ((key>=1 && key <=3) || (key==96-48 && !check))
  {
   if (key>=1 && key <=3) noPlayMode=key;
   if (total<=30000) total++;
   ShowPoint();
   Playing();
   out=0;
  }
  if (!out)
  {
   if (quit==1)
   {
    quit=0;
    first=1;
   }
   else if (quit==2)
   {
    quit=0;
    key=noPlayMode;
    ReloadBackground();
    ResetPoint();
    ShowPoint();
   }
   else
   {
    delay(100);
    Beep(100,100);
    Clr(_07);
    PutStr(1,25, 7,"[Esc] Quit ³ Anykey to playing ³> ");
    out=getch()-48;
    if (out==27-48) first=1;
    else if (out==96-48) key=out;
    else if (out==126-48) key=noPlayMode;
    else if ((win || quit || move==9) && !(out==96-48 || out==126-48)) ReloadBackground();
    else
    {
     out=0;
     key=noPlayMode;
     ReloadBackground();
    }
   }
  }
 } while (out!=1);
 End();
}