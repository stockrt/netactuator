/*
 * netmask.c
 * Copyright (C) 2000  Paul Davis, pdavis@lpccomp.bc.ca
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * This program takes an IP address and netmask from the command line
 * and shows the netmask, netmask bits, network address, broadcast address,
 * host IP range within the subnet and the number of such hosts.
 *
 *
 * I have made some modifications on this program, so I use it as a include
 * on netactuator, to filter IPs, matching against to the defined netmask to see if I
 * should or should not consider this IP for processing (filtering, graphing, you name it).
 * The accepted range is saved on the global vector networks_ranges[]
 * defs.h:unsigned long networks_ranges[MAX_ARGS][3]; // campo 0 = ip_inicial, campo 1 = ip_final, campo 2 = maskbits (inseridos somente na função netmask_paul_davis())
 *
 * Modification by: Rogério Carvalho Schneider (stockrt@gmail.com)
 * http://stockrt.github.com
 *
 */


/* Convert the given ip address in native byte order to a printable string */
char * my_ntoa(unsigned long ip)
{
	struct in_addr addr;


	addr.s_addr = htonl(ip);
	return inet_ntoa(addr);
}


// Converte a entrada ip/mask em valores para avaliação no sistema (long).
// Coloca esses dados em uma tabela.
void netmask_paul_davis (void)
{
	struct in_addr addr, netmask;
	unsigned long mask, network, hostmask, broadcast;
	int maskbits;
	int i, i_net;
	tipostring ip_rog, mask_rog;


	for (i_net=0; i_net<num_networks; i_net++)
	{
		// Usa a rede e mask definidas no netactuator.conf para conversão
		strcpy(ip_rog, networks[i_net][0]);
		strcpy(mask_rog, networks[i_net][1]);

		/* Convert the string ip address to network order address */
		if ( ! inet_aton(ip_rog, &addr) )
		{
			fprintf(stderr, "%s is not a valid IP address\n", ip_rog);
			exit(1);
		}

		/* If the netmask has a dot, assume it is the netmask as an ip address*/
		if ( strchr(mask_rog, '.' ) )
		{
			if ( ! inet_aton(mask_rog, &netmask) )
			{
				fprintf(stderr, "%s is not a valid IP netmask\n", mask_rog);
				exit(1);
			}
			/* Calculate the number of network bits */
			mask = ntohl(netmask.s_addr);
			for(maskbits=32; (mask & (1L<<(32-maskbits))) == 0; maskbits--)
				;
			/* Re-create the netmask and compare to the origianl
			 * to make sure it is a valid netmask.
			 */
			mask = 0;
			for(i=0; i<maskbits; i++)
				mask |= 1<<(31-i);
			if ( mask != ntohl(netmask.s_addr) )
			{
				fprintf(stderr, "%s is not a valid netmask, in hex:%x\n", mask_rog, ntohl(netmask.s_addr));
				exit(1);
			}
		}
		else
		{
			maskbits = atoi(mask_rog);
			if ( maskbits < 1 || maskbits > 30 )
			{
				fprintf(stderr, "Invalid net mask bits (1-30): %d\n", maskbits);
				exit(1);
			}
			/* Create the netmask from the number of bits */
			mask = 0;
			for(i=0; i<maskbits; i++)
				mask |= 1<<(31-i);
			netmask.s_addr = htonl(mask);
		}

		network = ntohl(addr.s_addr) & ntohl(netmask.s_addr);
		hostmask = ~ntohl(netmask.s_addr);
		broadcast = network | hostmask;

		// Salva os ranges de rede, ip_inicial e ip_final válidos
		networks_ranges[i_net][0] = network + 1;
		networks_ranges[i_net][1] = broadcast - 1;
		networks_ranges[i_net][2] = maskbits;
		strcpy(networks[i_net][1], inet_ntoa(netmask));
	} // for num_networks

/*
	printf("IP address   %s\n", inet_ntoa(addr));
	printf("Netmask      %s\n", inet_ntoa(netmask));
	printf("Netmask bits %d\n", maskbits);

	network = ntohl(addr.s_addr) & ntohl(netmask.s_addr);
	printf("Network      %s\n", my_ntoa(network));

	hostmask = ~ntohl(netmask.s_addr);
	broadcast = network | hostmask;
	printf("Broadcast    %s\n", my_ntoa(broadcast));
	printf("Hosts        %s\n", my_ntoa(network+1));
	printf("   to        %s\n", my_ntoa(broadcast-1));
	printf("Host count   %d\n", broadcast-network-1);
*/
}
