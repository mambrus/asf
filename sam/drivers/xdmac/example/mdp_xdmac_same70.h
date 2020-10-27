
#ifndef mdp_xdma_same70_h
#define mdp_xdma_same70_h

#include <xdmac.h>
#include <stdint.h>

#ifndef PACK
#define PACK( X ) __attribute__ ((packed)) X
#endif //PACK

typedef struct {
  uint32_t XDMAC_CIE;     /**< \brief (XdmacChid Offset: 0x0) Channel Interrupt Enable Register */
  uint32_t XDMAC_CID;     /**< \brief (XdmacChid Offset: 0x4) Channel Interrupt Disable Register */
  uint32_t XDMAC_CIM;     /**< \brief (XdmacChid Offset: 0x8) Channel Interrupt Mask Register */
  uint32_t XDMAC_CIS;     /**< \brief (XdmacChid Offset: 0xC) Channel Interrupt Status Register */
  uint32_t XDMAC_CSA;     /**< \brief (XdmacChid Offset: 0x10) Channel Source Address Register */
  uint32_t XDMAC_CDA;     /**< \brief (XdmacChid Offset: 0x14) Channel Destination Address Register */
  uint32_t XDMAC_CNDA;    /**< \brief (XdmacChid Offset: 0x18) Channel Next Descriptor Address Register */
  uint32_t XDMAC_CNDC;    /**< \brief (XdmacChid Offset: 0x1C) Channel Next Descriptor Control Register */
  uint32_t XDMAC_CUBC;    /**< \brief (XdmacChid Offset: 0x20) Channel Microblock Control Register */
  uint32_t XDMAC_CBC;     /**< \brief (XdmacChid Offset: 0x24) Channel Block Control Register */
  uint32_t XDMAC_CC;      /**< \brief (XdmacChid Offset: 0x28) Channel Configuration Register */
  uint32_t XDMAC_CDS_MSP; /**< \brief (XdmacChid Offset: 0x2C) Channel Data Stride Memory Set Pattern */
  uint32_t XDMAC_CSUS;    /**< \brief (XdmacChid Offset: 0x30) Channel Source Microblock Stride */
  uint32_t XDMAC_CDUS;    /**< \brief (XdmacChid Offset: 0x34) Channel Destination Microblock Stride */
  uint32_t Reserved1[2];
} mdp_XdmacChid;

typedef struct {
  uint32_t  XDMAC_GTYPE;                  /**< \brief (Xdmac Offset: 0x00) Global Type Register */
  uint32_t  XDMAC_GCFG;                   /**< \brief (Xdmac Offset: 0x04) Global Configuration Register */
  uint32_t  XDMAC_GWAC;                   /**< \brief (Xdmac Offset: 0x08) Global Weighted Arbiter Configuration Register */
  uint32_t  XDMAC_GIE;                    /**< \brief (Xdmac Offset: 0x0C) Global Interrupt Enable Register */
  uint32_t  XDMAC_GID;                    /**< \brief (Xdmac Offset: 0x10) Global Interrupt Disable Register */
  uint32_t  XDMAC_GIM;                    /**< \brief (Xdmac Offset: 0x14) Global Interrupt Mask Register */
  uint32_t  XDMAC_GIS;                    /**< \brief (Xdmac Offset: 0x18) Global Interrupt Status Register */
  uint32_t  XDMAC_GE;                     /**< \brief (Xdmac Offset: 0x1C) Global Channel Enable Register */
  uint32_t  XDMAC_GD;                     /**< \brief (Xdmac Offset: 0x20) Global Channel Disable Register */
  uint32_t  XDMAC_GS;                     /**< \brief (Xdmac Offset: 0x24) Global Channel Status Register */
  uint32_t  XDMAC_GRS;                    /**< \brief (Xdmac Offset: 0x28) Global Channel Read Suspend Register */
  uint32_t  XDMAC_GWS;                    /**< \brief (Xdmac Offset: 0x2C) Global Channel Write Suspend Register */
  uint32_t  XDMAC_GRWS;                   /**< \brief (Xdmac Offset: 0x30) Global Channel Read Write Suspend Register */
  uint32_t  XDMAC_GRWR;                   /**< \brief (Xdmac Offset: 0x34) Global Channel Read Write Resume Register */
  uint32_t  XDMAC_GSWR;                   /**< \brief (Xdmac Offset: 0x38) Global Channel Software Request Register */
  uint32_t  XDMAC_GSWS;                   /**< \brief (Xdmac Offset: 0x3C) Global Channel Software Request Status Register */
  uint32_t  XDMAC_GSWF;                   /**< \brief (Xdmac Offset: 0x40) Global Channel Software Flush Request Register */
  uint32_t  Reserved1[3];
  XdmacChid XDMAC_CHID[XDMACCHID_NUMBER]; /**< \brief (Xdmac Offset: 0x50) chid = 0 .. 23 */
  uint32_t  Reserved2[619];
  uint32_t  XDMAC_VERSION;                /**< \brief (Xdmac Offset: 0xFFC) XDMAC Version Register */
} mdp_Xdmac;


#endif /* mdp_xdma_same70_h */
