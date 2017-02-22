#include <time.h>
#include <systemc.h>

#include "router.h"
#include "pe.h"

int clk_cycle = 1;

SC_MODULE(top)
{
public:
	enum {N = 3};   //horizontally
	enum {M = 3};	//vertically
	
	router *routers[N][M];	//REVISED BY LLK
	PE_base *pes[N][M];		//REVISED BY LLK

	sc_signal<packet> router_to_pe[N][M], pe_to_router[N][M];
	sc_signal<packet> router_to_router_east[N][M-1], router_to_router_west[N][M-1];
	sc_signal<packet> router_to_router_north[N-1][M], router_to_router_south[N-1][M];
	
	sc_signal<packet> terminal_loop_east[N], terminal_loop_west[N];
	sc_signal<packet> terminal_loop_north[M], terminal_loop_south[M];
	sc_signal<bool> clock;

	SC_CTOR(top)
	{
		create_pes();
		create_network();
	}

protected:
	void create_pes()
	{
        pes[1][0] = new PE_P1("P1");
        pes[1][0]->clock(clock);
        pes[1][0]->set_xy(0, 1);
        
        pes[1][2] = new PE_P2("P2");
        pes[1][2]->clock(clock);
        pes[1][2]->set_xy(2, 1);
        
        pes[0][0] = new PE_P3("P3");
		pes[0][0]->clock(clock);
		pes[0][0]->set_xy(0, 0);
        
        pes[2][2] = new PE_P4("P4");
        pes[2][2]->clock(clock);
        pes[2][2]->set_xy(2, 2);
        
        pes[0][1] = new PE_P5("P5");
        pes[0][1]->clock(clock);
        pes[0][1]->set_xy(1, 0);
        
        pes[2][1] = new PE_P6("P6");
        pes[2][1]->clock(clock);
        pes[2][1]->set_xy(1, 2);
        
        pes[0][2] = new PE_P7("P7");
        pes[0][2]->clock(clock);
        pes[0][2]->set_xy(2, 0);
        
        pes[2][0] = new PE_P8("P8");
		pes[2][0]->clock(clock);
		pes[2][0]->set_xy(0, 2);

		pes[1][1] = new PE_IO("PI/PO");
		pes[1][1]->clock(clock);
		pes[1][1]->set_xy(1, 1);


		
	}

	void create_network()
	{
		for (int i = 0; i < M; ++i)
		{

			for (int j = 0; j < N; ++j)
			{
			char name[100];
			sprintf(name, "router(%d,%d)", i,j);

			// create router
			routers[j][i] = new router(name);
			routers[j][i]->set_xy(i,j);
			routers[j][i]->clock(clock);

			if(j!=0)	// connect router->north routers
			{
				routers[j][i]->port_out[router::NORTH](
					router_to_router_north[j-1][i]);
				routers[j][i]->port_in[router::NORTH](
					router_to_router_south[j-1][i]);
			}
			else	//or make a loop
			{
				routers[j][i]->port_out[router::NORTH](
					terminal_loop_north[i]);
				routers[j][i]->port_in[router::NORTH](
					terminal_loop_north[i]);
			}
			if(j !=N-1)	// connect router->south routers
			{
				routers[j][i]->port_out[router::SOUTH](
					router_to_router_south[j][i]);
				routers[j][i]->port_in[router::SOUTH](
					router_to_router_north[j][i]);
			}
			else	// or make a loop
			{
				routers[j][i]->port_out[router::SOUTH](
					terminal_loop_south[i]);
				routers[j][i]->port_in[router::SOUTH](
					terminal_loop_south[i]);
			}

			// connect router->west routers
			if (i != 0)
			{
				routers[j][i]->port_out[router::WEST](
					router_to_router_west[j][i-1]);
				routers[j][i]->port_in[router::WEST](
					router_to_router_east[j][i-1]);
			}
			else // or make a loop
			{
				routers[j][i]->port_out[router::WEST](
					terminal_loop_west[j]);
				routers[j][i]->port_in[router::WEST](
					terminal_loop_west[j]);
			}

			if (i != M-1) // connect router->east routers
			{
				routers[j][i]->port_out[router::EAST](
					router_to_router_east[j][i]);
				routers[j][i]->port_in[router::EAST](
					router_to_router_west[j][i]);
			}
			else // or make a loop
			{
				routers[j][i]->port_out[router::EAST](
					terminal_loop_east[j]);
				routers[j][i]->port_in[router::EAST](
					terminal_loop_east[j]);
			}

			// connect router->PE
			routers[j][i]->port_out[router::PE](router_to_pe[j][i]);
			routers[j][i]->port_in[router::PE](pe_to_router[j][i]);
			pes[j][i]->data_in(router_to_pe[j][i]);
			pes[j][i]->data_out(pe_to_router[j][i]);

			}
		}
	}

}; // top

int sc_main(int argc , char *argv[])
{
	srand(0);

	top top_module("top");

	printf("cycle  0 ================================\n");
	sc_start(0, SC_NS);

	for(; clk_cycle < 500; clk_cycle++){
		
		printf("cycle %2d ================================\n", clk_cycle);

		top_module.clock.write(1);
		sc_start(10, SC_NS);
		top_module.clock.write(0);
		sc_start(10, SC_NS);
	}
	extern int round_cycle;
	//printf("The minimum computation latency is %d.\n",round_cycle);
	extern int PE00_R,PE10_R,PE20_R,PE01_R,PE11_R,PE21_R,PE02_R,PE12_R,PE22_R;
	printf("The queue size from pe(1,1) to router(1,1) is %d.\n",PE11_R);
	printf("The queue size from pe(1,0) to router(1,0) is %d.\n",PE10_R);
	printf("The queue size from pe(2,1) to router(2,1) is %d.\n",PE21_R);
	printf("The queue size from pe(1,2) to router(1,2) is %d.\n",PE12_R);
	printf("The queue size from pe(0,1) to router(0,1) is %d.\n",PE01_R);
	printf("The queue size from pe(2,0) to router(2,0) is %d.\n",PE20_R);
	printf("The queue size from pe(0,2) to router(0,2) is %d.\n",PE02_R);
	printf("The queue size from pe(2,2) to router(2,2) is %d.\n",PE22_R);
	printf("The queue size from pe(0,0) to router(0,0) is %d.\n",PE00_R);
	extern int R00_PE,R00_East,R00_South;
    extern int R01_North,R01_East,R01_South,R01_PE;
    extern int R02_North,R02_East,R02_PE;
    extern int R10_West,R10_PE,R10_East,R10_South;
    extern int R11_North,R11_East,R11_South,R11_West,R11_PE;
    extern int R12_North,R12_East,R12_West,R12_PE;
    extern int R20_West,R20_South,R20_PE;
    extern int R21_North,R21_South,R21_West,R21_PE;
    extern int R22_North,R22_West,R22_PE;
	printf("The queue size from router(0,0) to router(1,0) is %d.\n",R00_East);
	printf("The queue size from router(0,0) to router(0,1) is %d.\n",R00_South);
	printf("The queue size from router(0,0) to pe(0,0) is %d.\n",R00_PE);
	printf("The queue size from router(1,0) to router(2,0) is %d.\n",R10_East);
	printf("The queue size from router(1,0) to router(1,1) is %d.\n",R10_South);
	printf("The queue size from router(1,0) to router(0,0) is %d.\n",R10_West);
	printf("The queue size from router(1,0) to pe(1,0) is %d.\n",R10_PE);
	printf("The queue size from router(2,0) to router(2,1) is %d.\n",R20_South);
	printf("The queue size from router(2,0) to router(1,0) is %d.\n",R20_West);
	printf("The queue size from router(2,0) to pe(2,0) is %d.\n",R20_PE);
	printf("The queue size from router(0,1) to router(0,0) is %d.\n",R01_North);
	printf("The queue size from router(0,1) to router(0,2) is %d.\n",R01_South);
	printf("The queue size from router(0,1) to router(1,1) is %d.\n",R01_East);
	printf("The queue size from router(0,1) to pe(0,1) is %d.\n",R01_PE);
	printf("The queue size from router(1,1) to router(0,1) is %d.\n",R11_West);
	printf("The queue size from router(1,1) to router(1,0) is %d.\n",R11_North);
	printf("The queue size from router(1,1) to router(2,1) is %d.\n",R11_East);
	printf("The queue size from router(1,1) to router(1,2) is %d.\n",R11_South);
	printf("The queue size from router(1,1) to pe(1,1) is %d.\n",R11_PE);
	printf("The queue size from router(2,1) to router(1,1) is %d.\n",R21_West);
	printf("The queue size from router(2,1) to router(2,0) is %d.\n",R21_North);
	printf("The queue size from router(2,1) to router(2,2) is %d.\n",R21_South);
	printf("The queue size from router(2,1) to pe(2,1) is %d.\n",R21_PE);
	printf("The queue size from router(0,2) to router(0,1) is %d.\n",R02_North);
	printf("The queue size from router(0,2) to router(1,2) is %d.\n",R02_East);
	printf("The queue size from router(0,2) to pe(0,2) is %d.\n",R02_PE);
	printf("The queue size from router(1,2) to router(1,1) is %d.\n",R12_North);
	printf("The queue size from router(1,2) to router(0,2) is %d.\n",R12_West);
	printf("The queue size from router(1,2) to router(2,2) is %d.\n",R12_East);
	printf("The queue size from router(1,2) to pe(1,2) is %d.\n",R12_PE);
	printf("The queue size from router(2,2) to router(1,2) is %d.\n",R22_West);
	printf("The queue size from router(2,2) to router(2,1) is %d.\n",R22_North);
	printf("The queue size from router(2,2) to pe(2,2) is %d.\n",R22_PE);
	extern int pofire;
	extern int clk_in;
	extern int clk_out;
	return 0;
}
