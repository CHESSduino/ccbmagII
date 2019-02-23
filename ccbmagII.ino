/*
  Character analysis operators

  Examples using the character analysis operators.
  Send any byte and the sketch will tell you about it.

  created 29 Nov 2010
  modified 2 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/CharacterAnalysis
*/

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // send an intro:
  Serial.println("Welcom CCB mag world.");
  Serial.println();
  backupboard();
}

char charbuf[80];
int pbuf = 0;
void updateboard();

void loop() {
  // get any incoming bytes:
  if (Serial.available() > 0) {
    int thisChar = Serial.read();

    // modify 0A line number to 0C.
    if ( thisChar == 10 && 
         pbuf == 1 &&
         charbuf[0] == 0x7E )
      thisChar = 12;

    /* say what was sent:
    Serial.print("You sent me: \'");
    Serial.print(thisChar,HEX);
    Serial.print("\'  ASCII Value: ");
    Serial.println(thisChar);
*/
    // check line end.
    if ( thisChar != 13 && thisChar != 10 )
    {
    //  Serial.println( "in thisChar != 13 && thisChar != 10 )" );
      charbuf[pbuf++] = thisChar;
    } else if ( pbuf > 0 )
    {
/*    //  charbuf[pbuf] = 0;
      // out the string.
      for ( int i=0; i<pbuf; i++ )
    //  while ( charbuf[i] != 0 )
      {
        Serial.print( charbuf[i], HEX );
        Serial.print( "," );
    //    i++;
      } Serial.println( "" );
*/      updateboard();
      // clear line.
      pbuf = 0;
    }
  }
}

char board[11*12] = 
{ '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0,
  '.','.','.','.',   '.','.','.','.',   '.','.','.',0 };
  
char bakboard[11*12]; 
void backupboard()
{
  memcpy( bakboard, board, sizeof(board) );
}
int countboard( char* p, char c, int* pos = NULL )
{
  int num = 0;
  for ( int i=0; i<sizeof(board); i++ )
  {
    if ( p[i] == c ) { num++; if ( pos != NULL ) *pos = i; }
  } return num;
}

void blackwhite()
{
  int bakXnum = countboard( bakboard, 'X' );
  int bakOnum = countboard( bakboard, 'O' );
  int Xnum = countboard( board, 'X' );
  int Onum = countboard( board, 'O' );
  int opos = 0;
  int onum = countboard( board, 'o', &opos );
  // search the diffrent.
  if ( onum == 1 ) 
  {
    if ( Xnum < Onum ) board[opos] = 'X';
    else board[opos] = 'O';
  }
  backupboard();
}

void updateboard()
{
  if ( pbuf > 0 && charbuf[0] == 0x7E && charbuf[1] >= 1 && charbuf[1] <= 12 )
  {
    // change the 12ln to 10ln.
    if ( charbuf[1] == 12 ) charbuf[1] = 10;
    // 
    int lntmp = charbuf[1]-1;
//    Serial.println( lntmp*12 );
    char* pl = board + lntmp*12;
    // 
    int left = ((unsigned char*)charbuf)[3];
    int right = ((unsigned char*)charbuf)[2];
//    Serial.println( left, HEX );
//    Serial.println( right, HEX );
    //
    if ( ( right/0x8 )*0x8 == ( right/0x4 )*0x4 ) pl[10] = '.'; else if ( pl[10] == '.' ) pl[10] = 'o'; 
    if ( ( right/0x4 )*0x4 == ( right/0x2 )*0x2 ) pl[9] = '.'; else if ( pl[9] == '.' ) pl[9] = 'o'; 
    if ( ( right/0x2 )*0x2 == ( right/0x1 )*0x1 ) pl[8] = '.'; else if ( pl[8] == '.' ) pl[8] = 'o'; 

    if ( ( left/0x100 )*0x100 == ( left/0x80 )*0x80 ) pl[7] = '.'; else if ( pl[7] == '.' ) pl[7] = 'o'; 
    if ( ( left/0x80 )*0x80 == ( left/0x40 )*0x40 ) pl[6] = '.'; else if ( pl[6] == '.' ) pl[6] = 'o'; 
    if ( ( left/0x40 )*0x40 == ( left/0x20 )*0x20 ) pl[5] = '.'; else if ( pl[5] == '.' ) pl[5] = 'o'; 
    if ( ( left/0x20 )*0x20 == ( left/0x10 )*0x10 ) pl[4] = '.'; else if ( pl[4] == '.' ) pl[4] = 'o'; 

    if ( ( left/0x10 )*0x10 == ( left/0x8 )*0x8 ) pl[3] = '.'; else if ( pl[3] == '.' ) pl[3] = 'o'; 
    if ( ( left/0x8 )*0x8 == ( left/0x4 )*0x4 ) pl[2] = '.'; else if ( pl[2] == '.' ) pl[2] = 'o'; 
    if ( ( left/0x4 )*0x4 == ( left/0x2 )*0x2 ) pl[1] = '.'; else if ( pl[1] == '.' ) pl[1] = 'o'; 
    if ( ( left/0x2 )*0x2 == ( left/0x1 )*0x1 ) pl[0] = '.'; else if ( pl[0] == '.' ) pl[0] = 'o'; 
    pl[11] = 0;
    // black and white pieces.
    blackwhite();
    // paint all board.
    for ( int i=0; i<11; i++ )
    {
      pl = board+(10-i)*12;
      for ( int j=0; j<i+1; j++ )
      {
        Serial.print( "  " );
      }
      for ( int j=0; j<11; j++ )
      {
        Serial.print( "   " );
        Serial.print( pl[j] );
      }
      Serial.println( "" );
    }
  }
}

