
#ifndef _SAACPLUS_BUFFCFG_
#define _SAACPLUS_BUFFCFG_

#define MAX_NCH		2
#define LN			2048
#define SN			256
#define LN2			(LN/2)
#define SN2			(SN/2)
#define NSHORT		(LN/SN)

#define MAXBANDLONG		64
#define MAXBANDSHORT	16
#define MAXBANDS		(MAXBANDSHORT*NSHORT)

#define MAX_BS_BUFF		(1024<<2)

#endif