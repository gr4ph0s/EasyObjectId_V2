#include "../main.h"
#include "OptionIO.h"

OptionIO::OptionIO()
{
	SetOptionPath();
	ReadOptionData();
}

//----------------------------------------------------------------------------------------
/// Set path member variable
//----------------------------------------------------------------------------------------
void OptionIO::SetOptionPath()
{
	// Set path folder
	optionFolderPath = GeGetStartupWritePath() + Filename("plugins") + Filename("EasyObjectId");
	optionFilePath = GeGetStartupWritePath() + Filename("plugins") + Filename("EasyObjectId") + Filename("Configuration.data");
}

//----------------------------------------------------------------------------------------
/// Check if an options file exist if not it create it.
/// @return										@trueIfOtherwiseFalse{successful}
//----------------------------------------------------------------------------------------
Bool OptionIO::CheckAndCreatePath()
{
	// If path do no exist we create it
	if (!GeFExist(optionFolderPath, true))
		if (!GeFCreateDirRec(optionFolderPath))
			return false;

	// If configuration files do not exist we create it
	if (!GeFExist(optionFilePath, false))
		if (!WriteOptionData(true))
			return false;

	return true;
}

//----------------------------------------------------------------------------------------
/// Set options and write them to the member FilePath variable.
/// @param[in] octaneOpt						The OctaneOptions to write
/// @param[in] vrayOpt							The VrayOptions to write
/// @param[in] redshiftOpt						The RedshiftOptions to write
/// @return										@trueIfOtherwiseFalse{successful}
//----------------------------------------------------------------------------------------
Bool OptionIO::WriteOptionData(OctaneOptions &octaneOpt, VrayOptions &vrayOpt, RedshiftOptions &redshiftOpt)
{
	octaneOptions = octaneOpt;
	vrayOptions = vrayOpt;
	redshiftOptions = redshiftOpt;
	return WriteOptionData();
}


//----------------------------------------------------------------------------------------
/// Write member option to the member FilePath variable.
/// @param[in] octaneOpt						The OctaneOptions to write
/// @param[in] vrayOpt							The VrayOptions to write
/// @param[in] redshiftOpt						The RedshiftOptions to write
/// @return										@trueIfOtherwiseFalse{successful}
//----------------------------------------------------------------------------------------
Bool OptionIO::WriteOptionData(Bool NeedCreation)
{
	if (!NeedCreation)
		if (!CheckAndCreatePath())
			return false;

	AutoAlloc<HyperFile> hf;
	if (!hf)
		return false;

	// Open the HyperFile
	if (!hf->Open(ID_CPP_EASYOBJECTID, optionFilePath, FILEOPEN_WRITE, FILEDIALOG_ANY))
		return false;

	// Vray
	if (!hf->WriteBool(vrayOptions.ConsiderForAA))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(vrayOptions.AffectMatteObject))
	{ hf->Close(); return false; }

	// Octane
	if (!hf->WriteBool(octaneOptions.UseObjectId))
	{hf->Close(); return false;}
	if (!hf->WriteBool(octaneOptions.UseLayerID))
	{ hf->Close(); return false; }

	// Redshift USE
	if (!hf->WriteBool(redshiftOptions.UseMpEnable))
	{hf->Close(); return false;}
	if (!hf->WriteBool(redshiftOptions.UseMpBitPersChannels))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.UseFoEnable))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.UseFoPath))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.UseFoJpegQuality))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.UseFoDwaCompression))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.UseFoDataType))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.UseFoFormat))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.UseFoBitPersChannels))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.UseFoCompression))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.UseFoStorage))
	{ hf->Close(); return false; }

	// Redshift
	if (!hf->WriteBool(redshiftOptions.ReflectRefractIds))
	{hf->Close(); return false;}
	if (!hf->WriteBool(redshiftOptions.MpEnable))
	{ hf->Close(); return false; }
	if (!hf->WriteInt32(redshiftOptions.MpBitPersChannels))
	{ hf->Close(); return false; }
	if (!hf->WriteBool(redshiftOptions.FoEnable))
	{ hf->Close(); return false; }
	if (!hf->WriteString(redshiftOptions.FoPath))
	{ hf->Close(); return false; }
	if (!hf->WriteInt32(redshiftOptions.FoJpegQuality))
	{ hf->Close(); return false; }
	if (!hf->WriteFloat32(redshiftOptions.FoDwaCompression))
	{ hf->Close(); return false; }
	if (!hf->WriteInt32(redshiftOptions.FoDataType))
	{hf->Close(); return false;}
	if (!hf->WriteInt32(redshiftOptions.FoFormat))
	{ hf->Close(); return false; }
	if (!hf->WriteInt32(redshiftOptions.FoBitPersChannels))
	{ hf->Close(); return false; }
	if (!hf->WriteInt32(redshiftOptions.FoCompression))
	{ hf->Close(); return false; }
	if (!hf->WriteInt32(redshiftOptions.FoStorage))
	{ hf->Close(); return false; }

	hf->Close();
	hf.Free();
	return true;
}

//----------------------------------------------------------------------------------------
/// Read member FilePath and set option member
/// @return										@trueIfOtherwiseFalse{successful}
//----------------------------------------------------------------------------------------
Bool OptionIO::ReadOptionData()
{
	if (!CheckAndCreatePath())
		return false;

	AutoAlloc<HyperFile> hf;
	if (!hf)
		return false;

	// Open the HyperFile
	if (!hf->Open(ID_CPP_EASYOBJECTID, optionFilePath, FILEOPEN_READ, FILEDIALOG_ANY))
		return false;

	Int32 BufferInt;
	Bool BufferBool;
	String BufferString;
	Float32 BufferFloat;

	// Vray
	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	vrayOptions.ConsiderForAA = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	vrayOptions.AffectMatteObject = BufferBool;

	// Octane
	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	octaneOptions.UseLayerID = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{ hf->Close(); return false; }
	octaneOptions.UseObjectId = BufferBool;

	// RedshiftUse
	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseMpEnable = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseMpBitPersChannels = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseFoEnable = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseFoPath = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseFoJpegQuality = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseFoDwaCompression = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseFoDataType = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseFoFormat = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseFoBitPersChannels = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseFoCompression = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;}
	redshiftOptions.UseFoStorage = BufferBool;


	// Redshift
	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false; }
	redshiftOptions.ReflectRefractIds = BufferBool;

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;	}
	redshiftOptions.MpEnable = BufferBool;

	if (!hf->ReadInt32(&BufferInt))
	{hf->Close(); return false;	}
	redshiftOptions.MpBitPersChannels = BitPersChannels(BufferInt);

	if (!hf->ReadBool(&BufferBool))
	{hf->Close(); return false;	}
	redshiftOptions.FoEnable = BufferBool;

	if (!hf->ReadString(&BufferString))
	{hf->Close(); return false;	}
	redshiftOptions.FoPath = BufferString;

	if (!hf->ReadInt32(&BufferInt))
	{hf->Close(); return false;	}
	redshiftOptions.FoJpegQuality = BufferInt;

	if (!hf->ReadFloat32(&BufferFloat))
	{hf->Close(); return false;	}
	redshiftOptions.FoDwaCompression = BufferFloat;

	if (!hf->ReadInt32(&BufferInt))
	{hf->Close(); return false;	}
	redshiftOptions.FoDataType = FoDataType(BufferInt);

	if (!hf->ReadInt32(&BufferInt))
	{hf->Close(); return false;	}
	redshiftOptions.FoFormat = FoFormat(BufferInt);

	if (!hf->ReadInt32(&BufferInt))
	{hf->Close(); return false;	}
	redshiftOptions.FoBitPersChannels = BitPersChannels(BufferInt);

	if (!hf->ReadInt32(&BufferInt))
	{hf->Close(); return false;	}
	redshiftOptions.FoCompression = FoCompression(BufferInt);

	if (!hf->ReadInt32(&BufferInt))
	{hf->Close(); return false; }
	redshiftOptions.FoStorage = FoStorage(BufferInt);

	hf->Close();
	hf.Free();
	return true;
}