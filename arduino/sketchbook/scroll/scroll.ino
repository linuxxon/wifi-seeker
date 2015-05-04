#include <scroll.h>

double dist=0;
void setup()
{
  Serial.begin(9600);
  mouse_init();
}

/*
 * get a reading from the mouse and report it back to the
 * host via the serial line.
 */
void loop()
{
  char mstat;
  char mx;
  char my;
  char mz;

  /* get a reading from the mouse */
  mouse_write(0xeb);  /* give me data! */
  mouse_read();      /* ignore ack */
  mstat = mouse_read();
  mx = mouse_read();
  my = mouse_read();
  mz = mouse_read();

  /* send the data back up */
  /*
  Serial.print(mstat, BIN);
  Serial.print("\tX=");
  Serial.print(mx, DEC);
  Serial.print("\tY=");
  Serial.print(my, DEC);
  Serial.print("\tZ=");
  Serial.print(mz, DEC);
  Serial.println();
  */
  dist+=(((double) mz)/330);
  if(mz!=0) {Serial.print(dist,DEC);Serial.println();}
  
  //delay(20);  /* twiddle */
}
