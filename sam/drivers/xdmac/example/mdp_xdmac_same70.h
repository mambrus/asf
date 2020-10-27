
#ifndef mdp_xdma_same70_h
#define mdp_xdma_same70_h

#include <xdmac.h>
#include <stdint.h>

#ifndef PACK
#define PACK( X ) __attribute__ ((packed)) X
#endif                          //PACK

typedef struct {
    uint32_t Block:1;           /* End of */
    uint32_t Linked_List:1;     /* End of */
    uint32_t Disable:1;         /* End of */
    uint32_t Flush:1;           /* End of */
    uint32_t Read_Bus_Error:1;
    uint32_t Write_Bus_Error:1;
    uint32_t Request_Overflow_Error:1;
    uint32_t _padding:25;
} mdp_XdmacIntr_t;

typedef struct {
    mdp_XdmacIntr_t CIE;        /* Channel interrupt Enable */
    mdp_XdmacIntr_t CID;        /* Channel interrupt Disable */
    mdp_XdmacIntr_t CIM;        /* Channel interrupt Mask */
    mdp_XdmacIntr_t CIS;        /* Channel interrupt Status */

    uint32_t CSA;
    uint32_t CDA;
    uint32_t CNDA;
    uint32_t CNDC;
    uint32_t CUBC;
    uint32_t CBC;
    uint32_t CC;
    uint32_t CDS_MSP;
    uint32_t CSUS;
    uint32_t CDUS;
    uint32_t Reserved1[2];
} mdp_XdmacChid_t;

typedef struct {
    uint32_t XDMAC_GTYPE;
    uint32_t XDMAC_GCFG;
    uint32_t XDMAC_GWAC;
    uint32_t XDMAC_GIE;
    uint32_t XDMAC_GID;
    uint32_t XDMAC_GIM;
    uint32_t XDMAC_GIS;
    uint32_t XDMAC_GE;
    uint32_t XDMAC_GD;
    uint32_t XDMAC_GS;
    uint32_t XDMAC_GRS;
    uint32_t XDMAC_GWS;
    uint32_t XDMAC_GRWS;
    uint32_t XDMAC_GRWR;
    uint32_t XDMAC_GSWR;
    uint32_t XDMAC_GSWS;
    uint32_t XDMAC_GSWF;
    uint32_t Reserved1[3];
    mdp_XdmacChid_t chid[XDMACCHID_NUMBER];
    uint32_t Reserved2[619];
    uint32_t XDMAC_VERSION;
} mdp_Xdmac_t;

extern mdp_Xdmac_t *mdp_Xdmac;

#endif                          /* mdp_xdma_same70_h */
