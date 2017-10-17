#ifndef GRAPH_EASYOBJECTID_OPTION_IO_H__
#define GRAPH_EASYOBJECTID_OPTION_IO_H__

#include "c4d.h"
#include "customgui_quicktab.h"
#include "Struct.h"

class OptionIO
{
	private:
		Filename optionFolderPath;
		Filename optionFilePath;
		OctaneOptions octaneOptions;
		VrayOptions vrayOptions;
		RedshiftOptions redshiftOptions;

		void SetOptionPath();
		Bool CheckAndCreatePath();

	public:
		OptionIO();
		OctaneOptions& GetOctaneOptions(){ return octaneOptions; };
		VrayOptions& GetVrayOptions(){ return vrayOptions; };
		RedshiftOptions& GetRedshiftOptions(){ return redshiftOptions; };


		Bool WriteOptionData(OctaneOptions &octaneOpt, VrayOptions &vrayOpt, RedshiftOptions &redshiftOpt);
		Bool WriteOptionData(Bool NeedCreation = false);
		Bool ReadOptionData();
};



#endif // GRAPH_EASYOBJECTID_OPTION_IO_H__