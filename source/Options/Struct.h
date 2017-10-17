#ifndef GRAPH_EASYOBJECTID_OPTIONS_STRUCT_H__
#define GRAPH_EASYOBJECTID_OPTIONS_STRUCT_H__

#include "c4d.h"

// Vray
struct VrayOptions
{
	Bool ConsiderForAA;
	Bool AffectMatteObject;

	VrayOptions() :ConsiderForAA(false), AffectMatteObject(true) {};
	VrayOptions(Bool aa, Bool matte) :
		ConsiderForAA(aa), AffectMatteObject(matte)
	{};
};

// Octane
struct OctaneOptions
{
	Bool UseObjectId;
	Bool UseLayerID;

	OctaneOptions() :UseObjectId(false), UseLayerID(true){};
	OctaneOptions(Bool objID, Bool LayerId) : 
		UseObjectId(objID), UseLayerID(LayerId)
		{};
};


// REDSHIFT
enum FoDataType
{
	rgb,
	rgba,
};

enum FoFormat
{
	exr,
	tiff,
	png,
	targa,
	jpeg
};

enum BitPersChannels
{
	b8,
	b16,
	b32,
	b16_5,
	b32_f,
};

enum FoCompression
{
	none,
	defaut,
	lzw = 100,
	ccitt_rle,
	zip_01,
	packbits,
	rle = 200,
	zip_02,
	zips,
	piz,
	pxr24,
	b44,
	dwaa,
	dwbb
};

enum FoStorage
{
	scanline,
	tiled,
};

struct RedshiftUseOptions
{
	Bool UseReflectRefractIds;

	// MultiPass
	Bool UseMpEnable;
	Bool UseMpBitPersChannels;

	// File Output
	Bool UseFoEnable;
	Bool UseFoPath;
	Bool UseFoJpegQuality;
	Bool UseFoDwaCompression;
	Bool UseFoDataType;
	Bool UseFoFormat;
	Bool UseFoBitPersChannels;
	Bool UseFoCompression;
	Bool UseFoStorage;

	RedshiftUseOptions() :
		UseReflectRefractIds(false),
		UseMpEnable(true),
		UseMpBitPersChannels(false),
		UseFoEnable(false),
		UseFoPath(false),
		UseFoJpegQuality(false),
		UseFoDwaCompression(false),
		UseFoDataType(false),
		UseFoFormat(false),
		UseFoBitPersChannels(false),
		UseFoCompression(false),
		UseFoStorage(false)
	{};

	~RedshiftUseOptions(){};
};

struct RedshiftOptions : RedshiftUseOptions
{
	Bool ReflectRefractIds;

	// MultiPass
	Bool MpEnable;
	BitPersChannels MpBitPersChannels;

	// File Output
	Bool FoEnable;
	String FoPath;
	Int32 FoJpegQuality;
	Float32 FoDwaCompression;
	FoDataType FoDataType;
	FoFormat FoFormat;
	BitPersChannels FoBitPersChannels;
	FoCompression FoCompression;
	FoStorage FoStorage;

	~RedshiftOptions(){};

	RedshiftOptions() :
		MpEnable(true),
		MpBitPersChannels(b32),
		FoEnable(false),
		FoPath("$filepath$filename_$pass"),
		FoJpegQuality(95),
		FoDwaCompression(float(45)),
		FoDataType(FoDataType::rgb),
		FoFormat(exr),
		FoBitPersChannels(b16_5),
		FoCompression(defaut),
		FoStorage(scanline)
	{};
};


#endif // GRAPH_EASYOBJECTID_OPTIONS_STRUCT_H__