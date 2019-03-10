/*************************************************************
*	Declaration of interrupt   			   		    		 *
*************************************************************/



/***************** fixed vector section *********************/

/* Voltage monitor2 interrupt */
#pragma INTERRUPT	wa2_int;
extern void	wa2_int(void);








/**************** variable vector section *******************/

/* Timer RD(Channel 0) interrupt(vector8) */
#pragma INTERRUPT	timer_rd0_int;
extern void	timer_rd0_int(void);

/* Timer RD(Channel 1) interrupt(vector9) */
#pragma INTERRUPT	timer_rd1_int;
extern void	timer_rd1_int(void);

/* Timer RE interrupt(vector10) */
#pragma INTERRUPT	timer_re_int;
extern void	timer_re_int(void);

/* Key in interrupt(vector 13) */
#pragma INTERRUPT	key_in_int;
extern void	key_in_int(void);

/* A/D interrupt(vector 14) */
#pragma INTERRUPT	ad_int;
extern void	ad_int(void);

/* SSU/IIC interrupt(vector 15) */
#pragma INTERRUPT	ssu_int;
extern void	ssu_int(void);

/* UART0 transmit interrupt(vector 17) */
#pragma INTERRUPT	uart0_tra_int;
extern void	uart0_tra_int(void);

/* UART0 receive interrupt(vector 18) */
#pragma INTERRUPT	uart0_rec_int;
extern void	uart0_rec_int(void);

/* UART1 transmit interrupt(vector 19) */
#pragma INTERRUPT	uart1_tra_int;
extern void	uart1_tra_int(void);

/* UART1 receive interrupt(vector 20) */
#pragma INTERRUPT	uart1_rec_int;
extern void	uart1_rec_int(void);

/* INT2 interrupt(vector 21) */
#pragma INTERRUPT	int2_int;
extern void	int2_int(void);

/* Timer RA interrupt(vector 22) */
#pragma INTERRUPT	timer_ra_int;
extern void	timer_ra_int(void);

/* Timer RB interrupt(vector 24) */
#pragma INTERRUPT	timer_rb_int;
extern void	timer_rb_int(void);

/* INT1 interrupt(vector 25) */
#pragma INTERRUPT	int1_int;
extern void	int1_int(void);

/* INT3 interrupt(vector 26) */
#pragma INTERRUPT	int3_int;
extern void	int3_int(void);

/* INT0 interrupt(vector 29) */
#pragma INTERRUPT	int0_int;
extern void	int0_int(void);
