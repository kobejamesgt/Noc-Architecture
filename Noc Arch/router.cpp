#include "router.h"


int R00_PE=0,R00_East=0,R00_South=0;
int R10_West=0,R10_PE=0,R10_East=0,R10_South=0;
int R20_West=0,R20_South=0,R20_PE=0;
int R01_North=0,R01_East=0,R01_South=0,R01_PE=0;
int R11_North=0,R11_East=0,R11_South=0,R11_West=0,R11_PE=0;
int R21_North=0,R21_South=0,R21_West=0,R21_PE=0;
int R02_North=0,R02_East=0,R02_PE=0;
int R12_North=0,R12_East=0,R12_West=0,R12_PE=0;
int R22_North=0,R22_West=0,R22_PE=0;

std::ostream &operator<<(std::ostream &o, const packet &p)
{
	char buf[100];
	sprintf(buf, "(%d,%d)->(%d,%d)",
		p.src_x, p.src_y, p.dest_x, p.dest_y);
	return o << buf << ", " << p.token.real<<p.token.imag;
}

void sc_trace(sc_trace_file *tf, const packet &p, const std::string &name)
{
	sc_trace(tf, p.src_x, name+".src.x");
	sc_trace(tf, p.src_y, name+".src.y");
	sc_trace(tf, p.dest_x, name+".dest.x");
	sc_trace(tf, p.dest_y, name+".dest.y");
	sc_trace(tf, p.token.real, name+".token.real");
	sc_trace(tf, p.token.imag, name+".token.imag");
}

void router::main()
{
	assert((x_ != -1) && (y_ != -1)); // to identify _PE

	for (int iport = 0; iport < PORTS; ++iport)
		read_packet(iport);

	for (int iport = 0; iport < PORTS; ++iport)
		write_packet(iport);
}

void router::set_xy(int x, int y)
{
	assert((x_ == -1) && (y_ == -1)); // set once only
	assert((x != -1) && (y != -1)); // must use a legal location

	x_ = x;
	y_ = y;
}

void router::read_packet(int iport)
{
	assert(iport < PORTS);

	packet p = port_in[iport].read();

	if ((p.src_x == -1) && (p.src_y == -1))
		return; // empty packet

	route_packet_xy(p);
}

void router::write_packet(int iport)
{
	assert(iport < PORTS);

	if (out_queue_[iport].empty())
	{
		port_out[iport].write(packet()); // write an empty packet
	}
	else
	{
		port_out[iport].write(out_queue_[iport].front());
		out_queue_[iport].pop_front();
	}
}

void router::route_packet_xy(packet p)
{
	if ((p.dest_x == -1) || (p.dest_y == -1))
	{
		printf("router (%d,%d): drop packet with invalid destination"
			" (%d,%d)->(%d,%d)\n",
			p.src_x, p.src_y, p.dest_x, p.dest_y);
		return;
	}

		if ((p.dest_x == x_) && (p.dest_y ==y_)) // to _PE
	{
		out_queue_[PE].push_back(p);
		if(x_==0&&y_==0&&R00_PE<(int)out_queue_[PE].size()) R00_PE=(int)out_queue_[PE].size();
		if(x_==1&&y_==0&&R10_PE<(int)out_queue_[PE].size()) R10_PE=(int)out_queue_[PE].size();
		if(x_==2&&y_==0&&R20_PE<(int)out_queue_[PE].size()) R20_PE=(int)out_queue_[PE].size();
		if(x_==0&&y_==1&&R01_PE<(int)out_queue_[PE].size()) R01_PE=(int)out_queue_[PE].size();
		if(x_==1&&y_==1&&R11_PE<(int)out_queue_[PE].size()) R11_PE=(int)out_queue_[PE].size();
		if(x_==2&&y_==1&&R21_PE<(int)out_queue_[PE].size()) R21_PE=(int)out_queue_[PE].size();
		if(x_==0&&y_==2&&R02_PE<(int)out_queue_[PE].size()) R02_PE=(int)out_queue_[PE].size();
		if(x_==1&&y_==2&&R12_PE<(int)out_queue_[PE].size()) R12_PE=(int)out_queue_[PE].size();
		if(x_==2&&y_==2&&R22_PE<(int)out_queue_[PE].size()) R22_PE=(int)out_queue_[PE].size();
	}
	if (p.dest_x > x_)   // right to EAST
	{
		out_queue_[EAST].push_back(p);
		if(x_==0&&y_==0&&R00_East<(int)out_queue_[EAST].size()) R00_East=(int)out_queue_[EAST].size();
		if(x_==0&&y_==1&&R01_East<(int)out_queue_[EAST].size()) R01_East=(int)out_queue_[EAST].size();
		if(x_==0&&y_==2&&R02_East<(int)out_queue_[EAST].size()) R02_East=(int)out_queue_[EAST].size();
		if(x_==1&&y_==0&&R10_East<(int)out_queue_[EAST].size()) R10_East=(int)out_queue_[EAST].size();
		if(x_==1&&y_==1&&R11_East<(int)out_queue_[EAST].size()) R11_East=(int)out_queue_[EAST].size();
		if(x_==1&&y_==2&&R12_East<(int)out_queue_[EAST].size()) R12_East=(int)out_queue_[EAST].size();
	}
	else if (p.dest_y > y_) // down to SOUTH
	{
		out_queue_[SOUTH].push_back(p);

		if(x_==0&&y_==0&&R00_South<(int)out_queue_[SOUTH].size()) R00_South=(int)out_queue_[SOUTH].size();
		if(x_==1&&y_==0&&R10_South<(int)out_queue_[SOUTH].size()) R10_South=(int)out_queue_[SOUTH].size();
		if(x_==2&&y_==0&&R20_South<(int)out_queue_[SOUTH].size()) R20_South=(int)out_queue_[SOUTH].size();
		if(x_==0&&y_==1&&R01_South<(int)out_queue_[SOUTH].size()) R01_South=(int)out_queue_[SOUTH].size();
		if(x_==1&&y_==1&&R11_South<(int)out_queue_[SOUTH].size()) R11_South=(int)out_queue_[SOUTH].size();
		if(x_==2&&y_==1&&R21_South<(int)out_queue_[SOUTH].size()) R21_South=(int)out_queue_[SOUTH].size();
	}
	else if (p.dest_x < x_)      // left to WEST
	{
		out_queue_[WEST].push_back(p);
		if(x_==1&&y_==0&&R10_West<(int)out_queue_[WEST].size()) R10_West=(int)out_queue_[WEST].size();
		if(x_==1&&y_==1&&R11_West<(int)out_queue_[WEST].size()) R11_West=(int)out_queue_[WEST].size();
		if(x_==1&&y_==2&&R12_West<(int)out_queue_[WEST].size()) R12_West=(int)out_queue_[WEST].size();
		if(x_==2&&y_==0&&R20_West<(int)out_queue_[WEST].size()) R20_West=(int)out_queue_[WEST].size();
		if(x_==2&&y_==1&&R21_West<(int)out_queue_[WEST].size()) R21_West=(int)out_queue_[WEST].size();
		if(x_==2&&y_==2&&R22_West<(int)out_queue_[WEST].size()) R22_West=(int)out_queue_[WEST].size();
	}
	else if (p.dest_y < y_) // up to NORTH
	{
		out_queue_[NORTH].push_back(p);
		if(x_==0&&y_==1&&R01_North<(int)out_queue_[NORTH].size()) R01_North=(int)out_queue_[NORTH].size();
		if(x_==1&&y_==1&&R11_North<(int)out_queue_[NORTH].size()) R11_North=(int)out_queue_[NORTH].size();
		if(x_==2&&y_==1&&R21_North<(int)out_queue_[NORTH].size()) R21_North=(int)out_queue_[NORTH].size();
		if(x_==0&&y_==2&&R02_North<(int)out_queue_[NORTH].size()) R02_North=(int)out_queue_[NORTH].size();
		if(x_==1&&y_==2&&R12_North<(int)out_queue_[NORTH].size()) R12_North=(int)out_queue_[NORTH].size();
		if(x_==2&&y_==2&&R22_North<(int)out_queue_[NORTH].size()) R22_North=(int)out_queue_[NORTH].size();
	}

}
