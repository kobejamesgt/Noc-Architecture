#include "pe.h"
#include <math.h>
extern int clk_cycle;
token_type x0in,x1in,x2in,x3in,x4in,x5in,x6in,x7in,
	       y0in,y1in,y2in,y3in,y4in,y5in,y6in,y7in,
		   y0out,y1out,y2out,y3out,y4out,y5out,y6out,y7out,
		   z0in,z1in,z2in,z3in,z4in,z5in,z6in,z7in,
		   z0out,z1out,z2out,z3out,z4out,z5out,z6out,z7out,
		   X0in,X1in,X2in,X3in,X4in,X5in,X6in,X7in,
		   X0out,X1out,X2out,X3out,X4out,X5out,X6out,X7out;

struct wstruct w1,w2,w3;
int pifire=1,pofire=0;
int round_cycle=-1;
int clk_in=0;
int clk_out=0;


int PE00_R=0,PE10_R=0,PE20_R=0,PE01_R=0,PE11_R=0,PE21_R=0,PE02_R=0,PE12_R=0,PE22_R=0;

void PE_base::set_xy(int x, int y)
{
	assert((x_ == -1) && (y_ == -1)); // set once only
	assert((x != -1) && (y != -1)); // must use a legal location
	
	x_ = x;
	y_ = y;
	w1.w_real=sqrt(2.0000)/2.0000;
	w1.w_imag=-sqrt(2.0000)/2.0000;
	w2.w_real=0.0000;
	w2.w_imag=-1.0000;
	w3.w_real=-sqrt(2.0000)/2.0000;
	w3.w_imag=-sqrt(2.0000)/2.0000;
}

void PE_base::read_input()
{
	packet_in_ = data_in.read();
}

void PE_base::write_output()
{
	if (out_queue_.empty())
	{
		data_out.write(packet());
	}
	else
	{
		data_out.write(out_queue_.front());
		out_queue_.pop_front();
	}
}

void PE_IO::execute()
{		//if(pifire<=1)
		//fire_PI();

	int r = rand()%100;
	if (r < 11)
		fire_PI();

	// fire PO if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire_PO();
}

void PE_IO::fire_PI()
{	
	token_type x0,x1,x2,x3,x4,x5,x6,x7;
	x0.real=(rand()%10000)/10000.0000; x0.imag=(rand()%10000)/10000.0000;
	x1.real=(rand()%10000)/10000.0000; x1.imag=(rand()%10000)/10000.0000;
	x2.real=(rand()%10000)/10000.0000; x2.imag=(rand()%10000)/10000.0000;
	x3.real=(rand()%10000)/10000.0000; x3.imag=(rand()%10000)/10000.0000;
	x4.real=(rand()%10000)/10000.0000; x4.imag=(rand()%10000)/10000.0000;
	x5.real=(rand()%10000)/10000.0000; x5.imag=(rand()%10000)/10000.0000;
	x6.real=(rand()%10000)/10000.0000; x6.imag=(rand()%10000)/10000.0000;
	x7.real=(rand()%10000)/10000.0000; x7.imag=(rand()%10000)/10000.0000;
	
	packet p0(x_, y_, 1, 0,pifire, "x0",x0);
	printf("Round %d:PI: send x0 %.4f+%.4fi to (%d,%d)\n",
		pifire,x0.real,x0.imag, p0.dest_x, p0.dest_y);
	packet p1(x_, y_, 1, 2,pifire, "x1",x1);
	printf("Round %d:PI: send x1 %.4f+%.4fi to (%d,%d)\n",
		pifire,x1.real,x1.imag, p1.dest_x, p1.dest_y);
	packet p2(x_, y_, 2, 1, pifire,"x2",x2);
	printf("Round %d:PI: send x2 %.4f+%.4fi to (%d,%d)\n",
		pifire,x2.real,x2.imag, p2.dest_x, p2.dest_y);
	packet p3(x_, y_, 0, 1,pifire,"x3", x3);
	printf("Round %d:PI: send x3 %.4f+%.4fi to (%d,%d)\n",
		pifire,x3.real,x3.imag, p3.dest_x, p3.dest_y);
	packet p4(x_, y_, 1, 0,pifire, "x4",x4);
	printf("Round %d:PI: send x4 %.4f+%.4fi to (%d,%d)\n",
		pifire,x4.real,x4.imag, p4.dest_x, p4.dest_y);
	packet p5(x_, y_, 1, 2,pifire,"x5", x5);
	printf("Round %d:PI: send x5 %.4f+%.4fi to (%d,%d)\n",
		pifire,x5.real,x5.imag, p5.dest_x, p5.dest_y);
	packet p6(x_, y_, 2, 1,pifire, "x6",x6);
	printf("Round %d:PI: send x6 %.4f+%.4fi to (%d,%d)\n",
		pifire,x6.real,x6.imag, p6.dest_x, p6.dest_y);
	packet p7(x_, y_, 0, 1,pifire, "x7",x7);
	printf("Round %d:PI: send x7 %.4f+%.4fi to (%d,%d)\n",
		pifire,x7.real,x7.imag, p7.dest_x, p7.dest_y);
	out_queue_.push_back(p0);
	
	out_queue_.push_back(p1);
	//++r11_south;
	out_queue_.push_back(p2);
	//++r11_east;
	out_queue_.push_back(p3);
	//++r11_west;
	out_queue_.push_back(p4);
	//++r11_north;
	out_queue_.push_back(p5);
	//++r11_south;
	out_queue_.push_back(p6);
	//++r11_east;
	out_queue_.push_back(p7);
	//++r11_west;
	if(pifire<529)
		clk_in=clk_in+clk_cycle;
	if(PE11_R<(int)out_queue_.size()) PE11_R=(int)out_queue_.size();
	
	pifire++;
}

void PE_IO::fire_PO()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));
	//pofire=poc/8+1;
	printf("Round %d:PO: receive %s = %.4f+%.4fi from (%d,%d)\n",
		packet_in_.round,packet_in_.content,packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,packet_in_.src_y);
	if(packet_in_.content=="X(7)")
		{
			pofire++;
			//round_cycle=clk_cycle;
			clk_out=clk_out+clk_cycle;
	}
	
}

void PE_P5::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire();
}

void PE_P5::fire()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));
	//token_type x0in,x4in,y0out,y1out;
	if(packet_in_.content=="x0")
	{
		printf("P5(%d,%d): receive x0 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	x0in.real=packet_in_.token.real;
	x0in.imag=packet_in_.token.imag;
	}
	if(packet_in_.content=="x4")
	{
	x4in.real=packet_in_.token.real;
	x4in.imag=packet_in_.token.imag;
	printf("P5(%d,%d): receive x4 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	
	y0out.real=x0in.real+x4in.real;
	y0out.imag=x0in.imag+x4in.imag;
	y1out.real=x0in.real-x4in.real;
	y1out.imag=x0in.imag-x4in.imag;
	packet py0(x_, y_, 0, 0,packet_in_.round, "y0",y0out);
	printf("P5 send y0=%.4f+%.4fi to (%d,%d)\n",
	py0.token.real,py0.token.imag, 0, 0);
	out_queue_.push_back(py0);
	packet py1(x_, y_, 2, 0,packet_in_.round,"y1", y1out);
	printf("P5 send y1=%.4f+%.4fi to (%d,%d)\n",
	py1.token.real,py1.token.imag, 2, 0);
	out_queue_.push_back(py1);
	if(PE10_R<(int)out_queue_.size()) PE10_R=(int)out_queue_.size();
	}
	if(packet_in_.content=="z1")
	{
		printf("P3(%d,%d): receive z1 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	z1in.real=packet_in_.token.real;
	z1in.imag=packet_in_.token.imag;
	}
	if(packet_in_.content=="z5")
	{
		printf("P7 (%d,%d): receive z5 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	z5in.real=packet_in_.token.real;
	z5in.imag=packet_in_.token.imag;
    X2out.real=z1in.real+z5in.imag;
	X2out.imag=z1in.imag-z5in.real;
	X6out.real=z1in.real-z5in.imag;
	X6out.imag=z1in.imag+z5in.real;
    packet pX2(x_, y_, 1, 1,packet_in_.round, "X(2)",X2out);
	printf("P5 send X2=%.4f+%.4fi to (%d,%d)\n",
	pX2.token.real,pX2.token.imag, 1, 1);
	out_queue_.push_back(pX2);
	packet pX6(x_, y_, 1, 1,packet_in_.round,"X(6)", X6out);
	printf("P5 send X6=%.4f+%.4fi to (%d,%d)\n",
	pX6.token.real,pX6.token.imag, 1, 1);
	out_queue_.push_back(pX6);
	if(PE10_R<(int)out_queue_.size()) PE10_R=(int)out_queue_.size();

	}

	
	
}

void PE_P2::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire();
}

void PE_P2::fire()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));

	if(packet_in_.content=="x2")
	{
		x2in.real=packet_in_.token.real;
		x2in.imag=packet_in_.token.imag;
		printf("P2(%d,%d): receive x2 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	}
	if(packet_in_.content=="x6")
	{
	x6in.real=packet_in_.token.real;
	x6in.imag=packet_in_.token.imag;
	printf("P2(%d,%d): receive x6 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	printf("x2=%.4f+%.4fi,x6=%.4f+%.4fi\n",x2in.real,x2in.imag,x6in.real,x6in.imag);
	y2out.real=x2in.real+x6in.real;
	y2out.imag=x2in.imag+x6in.imag;
	y3out.real=x2in.real-x6in.real;
	y3out.imag=x2in.imag-x6in.imag;
	printf("y2=%.4f+%.4fi,y3=%.4f+%.4fi\n",y2out.real,y2out.imag,y3out.real,y3out.imag);
	packet py2(x_, y_, 0, 0, packet_in_.round,"y2",y2out);
	printf("P2 send y2=%.4f+%.4fi to (%d,%d)\n",
	py2.token.real,py2.token.imag, 0, 0);
	out_queue_.push_back(py2);
	packet py3(x_, y_, 2, 0, packet_in_.round,"y3",y3out);
	printf("P2 send y3=%.4f+%.4fi to (%d,%d)\n",
	py3.token.real,py3.token.imag, 2, 0);
	out_queue_.push_back(py3);
	if(PE21_R<(int)out_queue_.size()) PE21_R=(int)out_queue_.size();
	}

	if(packet_in_.content=="z2")
	{
		printf("P2(%d,%d): receive z2 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	z2in.real=packet_in_.token.real;
	z2in.imag=packet_in_.token.imag;
	}
	if(packet_in_.content=="z6")
	{
	z6in.real=packet_in_.token.real;
	z6in.imag=packet_in_.token.imag;
	printf("P2(%d,%d): receive z6 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	
	
	X1out.real=z2in.real+(z6in.real*w1.w_real-z6in.imag*w1.w_imag);
	X1out.imag=z2in.imag+(z6in.real*w1.w_imag+z6in.imag*w1.w_real);
	X5out.real=z2in.real-(z6in.real*w1.w_real-z6in.imag*w1.w_imag);
	X5out.imag=z2in.imag-(z6in.real*w1.w_imag+z6in.imag*w1.w_real);
	
	packet pX1(x_, y_, 1, 1,packet_in_.round, "X(1)",X1out);
	printf("P2 send X1=%.4f+%.4fi to (%d,%d)\n",
	pX1.token.real,pX1.token.imag, 1, 1);
	out_queue_.push_back(pX1);
	packet pX5(x_, y_, 1, 1,packet_in_.round, "X(5)",X5out);
	printf("P2 send X5=%.4f+%.4fi to (%d,%d)\n",
	pX5.token.real,pX5.token.imag, 1, 1);
	out_queue_.push_back(pX5);
	if(PE21_R<(int)out_queue_.size()) PE21_R=(int)out_queue_.size();
	}

	
}

void PE_P6::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire();
}

void PE_P6::fire()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));

	if(packet_in_.content=="x1")
	{
		printf("P6(%d,%d): receive x1 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	x1in.real=packet_in_.token.real;
	x1in.imag=packet_in_.token.imag;
	}
	if(packet_in_.content=="x5")
	{
	x5in.real=packet_in_.token.real;
	x5in.imag=packet_in_.token.imag;
	printf("P6(%d,%d): receive x5 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	printf("x1=%.4f+%.4fi,x5=%.4f+%.4fi\n",x1in.real,x1in.imag,x5in.real,x5in.imag);
	
	y4out.real=x1in.real+x5in.real;
	y4out.imag=x1in.imag+x5in.imag;
	y5out.real=x1in.real-x5in.real;
	y5out.imag=x1in.imag-x5in.imag;
	packet py4(x_, y_, 0, 2,packet_in_.round, "y4",y4out);
	printf("P6 send y4=%.4f+%.4fi to (%d,%d)\n",
	py4.token.real,py4.token.imag, 0, 2);
	out_queue_.push_back(py4);
	packet py5(x_, y_, 2, 2,packet_in_.round,"y5", y5out);
	printf("P6 send y5=%.4f+%.4fi to (%d,%d)\n",
	py5.token.real,py5.token.imag, 2, 2);
	out_queue_.push_back(py5);
	if(PE12_R<(int)out_queue_.size()) PE12_R=(int)out_queue_.size();
	}

		if(packet_in_.content=="z3")
	{
		printf("P6(%d,%d): receive z3 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	z3in.real=packet_in_.token.real;
	z3in.imag=packet_in_.token.imag;
	}
	if(packet_in_.content=="z7")
	{
	z7in.real=packet_in_.token.real;
	z7in.imag=packet_in_.token.imag;
	printf("P6(%d,%d): receive z7 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	X3out.real=z3in.real+(z7in.real*w3.w_real-z7in.imag*w3.w_imag);
	X3out.imag=z3in.imag+(z7in.real*w3.w_imag+z7in.imag*w3.w_real);
	X7out.real=z3in.real-(z7in.real*w3.w_real-z7in.imag*w3.w_imag);
	X7out.imag=z3in.imag-(z7in.real*w3.w_imag+z7in.imag*w3.w_real);
	packet pX3(x_, y_, 1, 1,packet_in_.round,"X(3)", X3out);
	printf("P6 send x3=%.4f+%.4fi to (%d,%d)\n",
	pX3.token.real,pX3.token.imag, 1, 1);
	out_queue_.push_back(pX3);
	packet pX7(x_, y_, 1, 1,packet_in_.round, "X(7)",X7out);
	printf("P6 send x7=%.4f+%.4fi to (%d,%d)\n",
	pX7.token.real,pX7.token.imag, 1, 1);
	out_queue_.push_back(pX7);
	if(PE12_R<(int)out_queue_.size()) PE12_R=(int)out_queue_.size();
	}

	
}

void PE_P1::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire();
}

void PE_P1::fire()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));

	if(packet_in_.content=="x3")
	{
		printf("P1(%d,%d): receive x3 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	x3in.real=packet_in_.token.real;
	x3in.imag=packet_in_.token.imag;
	}
	if(packet_in_.content=="x7")
	{
	x7in.real=packet_in_.token.real;
	x7in.imag=packet_in_.token.imag;
	printf("P1(%d,%d): receive x7 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	
	y6out.real=x3in.real+x7in.real;
	y6out.imag=x3in.imag+x7in.imag;
	y7out.real=x3in.real-x7in.real;
	y7out.imag=x3in.imag-x7in.imag;
	packet py6(x_, y_, 0, 2, packet_in_.round,"y6",y6out);
	printf("P1 send y6=%.4f+%.4fi to (%d,%d)\n",
	py6.token.real,py6.token.imag, 0, 2);
	out_queue_.push_back(py6);
	packet py7(x_, y_, 2, 2,packet_in_.round,"y7", y7out);
	printf("P1 send y7=%.4f+%.4fi to (%d,%d)\n",
	py7.token.real,py7.token.imag, 2, 2);
	out_queue_.push_back(py7);
	if(PE01_R<(int)out_queue_.size()) PE01_R=(int)out_queue_.size();
	}

	if(packet_in_.content=="z0")
	{
		printf("P1(%d,%d): receive z0 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	z0in.real=packet_in_.token.real;
	z0in.imag=packet_in_.token.imag;
	}
	if(packet_in_.content=="z4")
	{
		printf("P1(%d,%d): receive z4 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	z4in.real=packet_in_.token.real;
	z4in.imag=packet_in_.token.imag;

	X0out.real=z0in.real+z4in.real;
	X0out.imag=z0in.imag+z4in.imag;
	X4out.real=z0in.real-z4in.real;
	X4out.imag=z0in.imag-z4in.imag;

	packet pX0(x_, y_, 1, 1,packet_in_.round,"X(0)", X0out);
	printf("P1 send X0=%.4f+%.4fi to (%d,%d)\n",
	pX0.token.real,pX0.token.imag, 1, 1);
	out_queue_.push_back(pX0);
	packet pX4(x_, y_, 1, 1,packet_in_.round, "X(4)",X4out);
	printf("P1 send X4=%.4f+%.4fi to (%d,%d)\n",
	pX4.token.real,pX4.token.imag, 1, 1);
	out_queue_.push_back(pX4);
	if(PE01_R<(int)out_queue_.size()) PE01_R=(int)out_queue_.size();
	}

	
}

void PE_P7::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire();
}
void PE_P7::fire()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));


	if(packet_in_.content=="y1")
	{
		printf("P7(%d,%d): receive y1 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	y1in.real=packet_in_.token.real;
	y1in.imag=packet_in_.token.imag;
	}

	if(packet_in_.content=="y3")
	{
	y3in.real=packet_in_.token.real;
	y3in.imag=packet_in_.token.imag;
	printf("P7(%d,%d): receive y3 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	
	z2out.real=y1in.real+y3in.imag;
	z2out.imag=y1in.imag-y3in.real;
	z3out.real=y1in.real-y3in.imag;
	z3out.imag=y1in.imag+y3in.real;

	packet pz2(x_, y_, 2, 1,packet_in_.round, "z2",z2out);
	printf("P7 send z2=%.4f+%.4fi to (%d,%d)\n",
	pz2.token.real,pz2.token.imag, 2, 1);
	out_queue_.push_back(pz2);
	packet pz3(x_, y_, 1, 2,packet_in_.round, "z3",z3out);
	printf("P7 send z3=%.4f+%.4fi to (%d,%d)\n",
	pz3.token.real,pz3.token.imag, 1, 2);
	out_queue_.push_back(pz3);
	if(PE20_R<(int)out_queue_.size()) PE20_R=(int)out_queue_.size();
	}

	
}

void PE_P8::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))

		fire();
}
void PE_P8::fire()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));

	if(packet_in_.content=="y4")
	{
		printf("P8(%d,%d): receive y4 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	y4in.real=packet_in_.token.real;
	y4in.imag=packet_in_.token.imag;
	}
	if(packet_in_.content=="y6")
	{
		printf("P8(%d,%d): receive y6 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	y6in.real=packet_in_.token.real;
	y6in.imag=packet_in_.token.imag;

	z4out.real=y4in.real+y6in.real;
	z4out.imag=y4in.imag+y6in.imag;
	z5out.real=y4in.real-y6in.real;
	z5out.imag=y4in.imag-y6in.imag;

	packet pz4(x_, y_, 0, 1,packet_in_.round,"z4", z4out);
	printf("P8 send z4=%.4f+%.4fi to (%d,%d)\n",
	pz4.token.real,pz4.token.imag, 0, 1);
	out_queue_.push_back(pz4);
	packet pz5(x_, y_, 1, 0,packet_in_.round, "z5",z5out);
	printf("P8 send z5=%.4f+%.4fi to (%d,%d)\n",
	pz5.token.real,pz5.token.imag, 1, 0);
	out_queue_.push_back(pz5);
	if(PE02_R<(int)out_queue_.size()) PE02_R=(int)out_queue_.size();
	}
	
	
	
}

void PE_P4::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))
		fire();
}
void PE_P4::fire()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));


	if(packet_in_.content=="y5")
	{
		printf("P4(%d,%d): receive y5 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	y5in.real=packet_in_.token.real;
	y5in.imag=packet_in_.token.imag;
	}
if(packet_in_.content=="y7")
	{
	y7in.real=packet_in_.token.real;
	y7in.imag=packet_in_.token.imag;
	printf("P4(%d,%d): receive y7 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	
	z6out.real=y5in.real+y7in.imag;
	z6out.imag=y5in.imag-y7in.real;
	z7out.real=y5in.real-y7in.imag;
	z7out.imag=y5in.imag+y7in.real;
	
	packet pz6(x_, y_, 2, 1,packet_in_.round, "z6",z6out);
	printf("P4 send z6=%.4f+%.4fi to (%d,%d)\n",
	pz6.token.real,pz6.token.imag, 2, 1);
	out_queue_.push_back(pz6);
	packet pz7(x_, y_, 1, 2,packet_in_.round, "z7",z7out);
	printf("P4 send z7=%.4f+%.4fi to (%d,%d)\n",
	pz7.token.real,pz7.token.imag, 1, 2);
	out_queue_.push_back(pz7);
	if(PE22_R<(int)out_queue_.size()) PE22_R=(int)out_queue_.size();
	}
	

}

void PE_P3::execute()
{
	// fire the actor if the incoming packet is valid
	if ((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1))

		fire();
}
void PE_P3::fire()
{
	assert((packet_in_.src_x != -1)
		&& (packet_in_.src_y != -1));


	if(packet_in_.content=="y0")
	{
		printf("P3(%d,%d): receive y0 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	y0in.real=packet_in_.token.real;
	y0in.imag=packet_in_.token.imag;
	}
if(packet_in_.content=="y2")
	{
		printf("P3(%d,%d): receive y2 %.4f+%.4fi from (%d,%d)\n",
		x_, y_,
		packet_in_.token.real,packet_in_.token.imag,packet_in_.src_x,	packet_in_.src_y
		);
	y2in.real=packet_in_.token.real;
	y2in.imag=packet_in_.token.imag;

z0out.real=y0in.real+y2in.real;
	z0out.imag=y0in.imag+y2in.imag;
	z1out.real=y0in.real-y2in.real;
	z1out.imag=y0in.imag-y2in.imag;

packet pz0(x_, y_, 0, 1,packet_in_.round, "z0",z0out);
	printf("P3 send z0=%.4f+%.4fi to (%d,%d)\n",
	pz0.token.real,pz0.token.imag, 0, 1);
	out_queue_.push_back(pz0);
	packet pz1(x_, y_, 1, 0,packet_in_.round,"z1", z1out);
	printf("P3 send z1=%.4f+%.4fi to (%d,%d)\n",
	pz1.token.real,pz1.token.imag, 1, 0);
	out_queue_.push_back(pz1);
	if(PE00_R<(int)out_queue_.size()) PE00_R=(int)out_queue_.size();

	}
	
	
}
