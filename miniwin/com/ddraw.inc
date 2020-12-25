typedef struct _DDCOLORKEY {
	DWORD dwColorSpaceLowValue;
	DWORD dwColorSpaceHighValue;
} DDCOLORKEY;

typedef struct _DDSCAPS {
	DWORD dwCaps;
} DDSCAPS, *LPDDSCAPS;

typedef struct _DDPIXELFORMAT {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwFourCC;
	union {
		DWORD dwRGBBitCount;
		DWORD dwYUVBitCount;
		DWORD dwZBufferBitDepth;
		DWORD dwAlphaBitDepth;
	};
	union {
		DWORD dwRBitMask;
		DWORD dwYBitMask;
	};
	union {
		DWORD dwGBitMask;
		DWORD dwUBitMask;
	};
	union {
		DWORD dwBBitMask;
		DWORD dwVBitMask;
	};
	union {
		DWORD dwRGBAlphaBitMask;
		DWORD dwYUVAlphaBitMask;
		DWORD dwRGBZBitMask;
		DWORD dwYUVZBitMask;
	};
} DDPIXELFORMAT, *LPDDPIXELFORMAT;

typedef struct _DDSURFACEDESC {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwHeight;
	DWORD dwWidth;
	union {
		LONG lPitch;
		DWORD dwLinearSize;
	};
	DWORD dwBackBufferCount;
	union {
		DWORD dwMipMapCount;
		DWORD dwZBufferBitDepth;
		DWORD dwRefreshRate;
	};
	DWORD dwAlphaBitDepth;
	DWORD dwReserved;
	LPVOID lpSurface;
	DDCOLORKEY ddckCKDestOverlay;
	DDCOLORKEY ddckCKDestBlt;
	DDCOLORKEY ddckCKSrcOverlay;
	DDCOLORKEY ddckCKSrcBlt;
	DDPIXELFORMAT ddpfPixelFormat;
	DDSCAPS ddsCaps;
} DDSURFACEDESC, *LPDDSURFACEDESC;

typedef struct {
	WORD	red[256];
	WORD	green[256];
	WORD	blue[256];
} DDGAMMARAMP,*LPDDGAMMARAMP;



typedef struct _DDBLTFX
{
    DWORD       dwSize;                         /* size of structure */
    DWORD       dwDDFX;                         /* FX operations */
    DWORD       dwROP;                          /* Win32 raster operations */
    DWORD       dwDDROP;                        /* Raster operations new for DirectDraw */
    DWORD       dwRotationAngle;                /* Rotation angle for blt */
    DWORD       dwZBufferOpCode;                /* ZBuffer compares */
    DWORD       dwZBufferLow;                   /* Low limit of Z buffer */
    DWORD       dwZBufferHigh;                  /* High limit of Z buffer */
    DWORD       dwZBufferBaseDest;              /* Destination base value */
    DWORD       dwZDestConstBitDepth;           /* Bit depth used to specify Z constant for destination */
    union
    {
        DWORD   dwZDestConst;                   /* Constant to use as Z buffer for dest */
        struct IDirectDrawSurface *lpDDSZBufferDest; /* Surface to use as Z buffer for dest */
    } DUMMYUNIONNAME1;
    DWORD       dwZSrcConstBitDepth;            /* Bit depth used to specify Z constant for source */
    union
    {
        DWORD   dwZSrcConst;                    /* Constant to use as Z buffer for src */
        struct IDirectDrawSurface *lpDDSZBufferSrc; /* Surface to use as Z buffer for src */
    } DUMMYUNIONNAME2;
    DWORD       dwAlphaEdgeBlendBitDepth;       /* Bit depth used to specify constant for alpha edge blend */
    DWORD       dwAlphaEdgeBlend;               /* Alpha for edge blending */
    DWORD       dwReserved;
    DWORD       dwAlphaDestConstBitDepth;       /* Bit depth used to specify alpha constant for destination */
    union
    {
        DWORD   dwAlphaDestConst;               /* Constant to use as Alpha Channel */
        struct IDirectDrawSurface *lpDDSAlphaDest; /* Surface to use as Alpha Channel */
    } DUMMYUNIONNAME3;
    DWORD       dwAlphaSrcConstBitDepth;        /* Bit depth used to specify alpha constant for source */
    union
    {
        DWORD   dwAlphaSrcConst;                /* Constant to use as Alpha Channel */
        struct IDirectDrawSurface *lpDDSAlphaSrc; /* Surface to use as Alpha Channel */
    } DUMMYUNIONNAME4;
        DWORD   dwFillColor;                    /* color in RGB or Palettized */
        DWORD   dwFillDepth;                    /* depth value for z-buffer */
        DWORD   dwFillPixel;                    /* pixel val for RGBA or RGBZ */
        struct IDirectDrawSurface *lpDDSPattern; /* Surface to use as pattern */
    DDCOLORKEY  ddckDestColorkey;               /* DestColorkey override */
    DDCOLORKEY  ddckSrcColorkey;                /* SrcColorkey override */
} DDBLTFX,*LPDDBLTFX;

typedef struct IDirectDrawPalette *LPDIRECTDRAWPALETTE;
typedef struct IDirectDrawSurface *LPDIRECTDRAWSURFACE;
typedef struct IDirectDraw *LPDIRECTDRAW;
typedef struct IDirectDrawGammaControl *LPDIRECTDRAWGAMMACONTROL;

// No methods are actually used
DECLARE_INTERFACE_(IDirectDrawPalette, IUnknown)
{
	// clang-format off
	STDMETHOD(GetCaps)(THIS_ LPDWORD lpdwCaps) PURE;
	STDMETHOD(GetEntries)(THIS_ DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries) PURE;
	STDMETHOD(SetEntries)(THIS_ DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries) PURE;
	// clang-format on
};

DECLARE_INTERFACE_(IDirectDrawSurface, IUnknown)
{
	// clang-format off
	STDMETHOD(BltFast)(THIS_ DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans) PURE;
	STDMETHOD(GetCaps)(THIS_ LPDDSCAPS lpDDSCaps) PURE;
	STDMETHOD(GetDC)(THIS_ HDC *lphDC) PURE;
	STDMETHOD(GetPixelFormat)(THIS_ LPDDPIXELFORMAT lpDDPixelFormat) PURE;
	STDMETHOD(IsLost)(THIS) PURE;
	STDMETHOD(Lock)(THIS_ LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent) PURE;
	STDMETHOD(ReleaseDC)(THIS_ HDC hDC) PURE;
	STDMETHOD(Restore)(THIS) PURE;
	STDMETHOD(SetPalette)(THIS_ LPDIRECTDRAWPALETTE lpDDPalette) PURE;
	STDMETHOD(Unlock)(THIS_ LPVOID lpSurfaceData) PURE;
	// clang-format off
};

typedef void *LPDDCAPS;
DECLARE_INTERFACE_(IDirectDraw,IUnknown)
{
	// clang-format off
	STDMETHOD(CreatePalette)(THIS_ DWORD dwFlags, LPPALETTEENTRY lpColorTable, LPDIRECTDRAWPALETTE *lplpDDPalette, IUnknown *pUnkOuter) PURE;
	STDMETHOD(CreateSurface)(THIS_ LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE *lplpDDSurface, IUnknown *pUnkOuter) PURE;
	STDMETHOD(SetCooperativeLevel)(THIS_ HWND hWnd, DWORD dwFlags) PURE;
	STDMETHOD(SetDisplayMode)(THIS_ DWORD dwWidth, DWORD dwHeight, DWORD dwBPP) PURE;
	STDMETHOD(WaitForVerticalBlank)(THIS_ DWORD dwFlags, HANDLE hEvent) PURE;
	// clang-format on
};

DECLARE_INTERFACE_(IDirectDrawGammaControl,IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD_(HRESULT,QueryInterface)(THIS_ REFIID riid, void** ppvObject) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    /*** IDirectDrawGammaControl methods ***/
    STDMETHOD(GetGammaRamp)(THIS_ DWORD flags, DDGAMMARAMP *gamma_ramp) PURE;
    STDMETHOD(SetGammaRamp)(THIS_ DWORD flags, DDGAMMARAMP *gamma_ramp) PURE;
};

