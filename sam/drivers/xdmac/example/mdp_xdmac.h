#ifndef mdp_xdma_h
#define mdp_xdma_h

#if defined(__same70q21__) || defined(__same70q21a__) | defined(__same70q21b__) || \
    defined(__same70n21__) || defined(__same70n21a__) | defined(__same70n21b__) || \
    defined(__same70j21__) || defined(__same70j21a__) | defined(__same70j21b__)

#  include "mdp_xdmac_same70.h"

#else
#  error mdp_sam is not ported to part PART
#endif

#endif /* mdp_xdma_h */
