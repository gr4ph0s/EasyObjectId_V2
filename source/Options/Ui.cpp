#include "Ui.h"
#include "Struct.h"

Bool Ui::CreateLayout()
{
	Bool res = GeDialog::CreateLayout();
	SetTitle("EasyObject ID v2 - Settings");

	if (GroupBegin(1000, BFH_SCALEFIT | BFV_SCALEFIT, 1, 0, "", 0, 0, 0))
	{
		// tab parent group within the left subgroup
		if (TabGroupBegin(1001, BFH_SCALEFIT | BFV_SCALEFIT))
		{
			// Vray
			if (GroupBegin(1100, BFH_SCALEFIT | BFV_SCALEFIT, 1, 0, "Vray Settings", 0, 400, 300))
			{
				AddCheckbox(1101, BFH_LEFT, 0, 10, "Consider For AA");
				AddCheckbox(1102, BFH_LEFT, 0, 10, "Affect Matte Object");
				GroupEnd();
			}

			// Octane
			if (GroupBegin(1200, BFH_SCALEFIT | BFV_SCALEFIT, 1, 0, "Octane Settings", 0, 400, 300))
			{
				AddCheckbox(1201, BFH_LEFT, 0, 10, "Define Object ID");
				AddCheckbox(1202, BFH_LEFT, 0, 10, "Define Layer ID");
				GroupEnd();
			}

			// Redshift
			if (GroupBegin(1300, BFH_SCALEFIT | BFV_SCALEFIT, 1, 0, "Redshift Settings", 0, 400, 300))
			{
				AddCheckbox(1301, BFH_LEFT, 0, 10, "Reflect/Refract IDs");

				// Multi-Pass Output
				BaseContainer bc = BaseContainer();
				bc.SetInt32(QUICKTAB_BAR, 1);
				bc.SetString(QUICKTAB_BARTITLE, "Multi-Pass Output");
				bc.SetBool(QUICKTAB_BARSUBGROUP, true);
				_quickTabRedshiftMP = static_cast<QuickTabCustomGui*>(AddCustomGui(1400, CUSTOMGUI_QUICKTAB, "", BFH_SCALEFIT | BFV_TOP, 0, 5, bc));
				if (!_quickTabRedshiftMP)
					return false;

				if (GroupBegin(1401, BFH_SCALEFIT | BFV_TOP, 1, 0, "Multi-Pass Output", 0, 0, 0))
				{
					GroupBorderSpace(10, 0, 10, 0);
					// Enable
					if (GroupBegin(1410, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{

						AddCheckbox(1411, BFH_LEFT, 0, 10, "");
						AddStaticText(1412, BFH_LEFT, 0, 10, "Enable", 0);
						AddCheckbox(1413, BFH_LEFT, 0, 10, "");
						GroupEnd();
					}

					// Bits
					if (GroupBegin(1420, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{
						AddCheckbox(1421, BFH_LEFT, 0, 10, "");
						AddStaticText(1422, BFH_LEFT, 0, 10, "Bits Pers Channels", 0);
						AddComboBox(1423, BFH_LEFT, 100, 10, false);
						AddChild(1423, 0, "8 Bits");
						AddChild(1423, 1, "16 Bits");
						AddChild(1423, 2, "32 Bits");
						GroupEnd();
					}
					GroupEnd();
				}

				// File Output
				bc = BaseContainer();
				bc.SetInt32(QUICKTAB_BAR, 1);
				bc.SetString(QUICKTAB_BARTITLE, "File Output");
				bc.SetBool(QUICKTAB_BARSUBGROUP, true);
				_quickTabRedshiftFO = static_cast<QuickTabCustomGui*>(AddCustomGui(1500, CUSTOMGUI_QUICKTAB, "", BFH_SCALEFIT | BFV_TOP, 0, 5, bc));
				if (!_quickTabRedshiftMP)
					return false;

				if (GroupBegin(1501, BFH_SCALEFIT | BFV_TOP, 1, 0, "File Output", 0, 0, 0))
				{
					GroupBorderSpace(10, 0, 10, 0);
					// Enable
					if (GroupBegin(1510, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{

						AddCheckbox(1511, BFH_LEFT, 0, 10, "");
						AddStaticText(1512, BFH_LEFT, 0, 10, "Enable", 0);
						AddCheckbox(1513, BFH_SCALEFIT, 0, 10, "");
						GroupEnd();
					}
					// Path
					if (GroupBegin(1520, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{
						AddCheckbox(1521, BFH_LEFT, 0, 10, "");
						AddStaticText(1522, BFH_LEFT, 0, 10, "Path", 0);
						AddEditText(1523, BFH_SCALEFIT, 0, 10, 0);
						GroupEnd();
					}
					// Data Type
					if (GroupBegin(1530, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{
						AddCheckbox(1531, BFH_LEFT, 0, 10, "");
						AddStaticText(1532, BFH_LEFT, 0, 10, "Data Type", 0);
						AddComboBox(1533, BFH_SCALEFIT, 100, 10, false);
						AddChild(1533, 0, "RGB");
						AddChild(1533, 1, "RGBA");
						GroupEnd();
					}
					// Format
					if (GroupBegin(1540, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{
						AddCheckbox(1541, BFH_LEFT, 0, 10, "");
						AddStaticText(1542, BFH_LEFT, 0, 10, "Format", 0);
						AddComboBox(1543, BFH_SCALEFIT, 100, 10, false);
						AddChild(1543, 0, "OpenEXR");
						AddChild(1543, 1, "TIFF");
						AddChild(1543, 2, "PNG");
						AddChild(1543, 3, "TARGA");
						AddChild(1543, 4, "JPEG");
						GroupEnd();
					}
					// Bits
					if (GroupBegin(1550, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{
						AddCheckbox(1551, BFH_LEFT, 0, 10, "");
						AddStaticText(1552, BFH_LEFT, 0, 10, "Bits Pers Channels", 0);
						AddComboBox(1553, BFH_SCALEFIT, 100, 10, false);
						AddChild(1553, 0, "8 Bits");
						AddChild(1553, 1, "16 Bits");
						AddChild(1553, 2, "32 Bits");
						AddChild(1553, 3, "Half Float (16 Bits)");
						AddChild(1553, 4, "Float (32 Bits)");
						GroupEnd();
					}
					// Compression
					if (GroupBegin(1560, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{
						AddCheckbox(1561, BFH_LEFT, 0, 10, "");
						AddStaticText(1562, BFH_LEFT, 0, 10, "Compression", 0);
						AddComboBox(1563, BFH_SCALEFIT, 100, 10, false);
						AddChild(1563, 0, "None");
						AddChild(1563, 1, "Default");
						AddChild(1563, 100, "LZW");
						AddChild(1563, 101, "CCITT RLE");
						AddChild(1563, 102, "ZIP");
						AddChild(1563, 103, "PackBits");
						AddChild(1563, 200, "RLE");
						AddChild(1563, 201, "ZIP");
						AddChild(1563, 202, "ZIPS");
						AddChild(1563, 203, "PIZ");
						AddChild(1563, 204, "PXR24");
						AddChild(1563, 205, "B44");
						AddChild(1563, 206, "DWAA");
						AddChild(1563, 207, "DWBB");
						GroupEnd();
					}
					// DWA Compression
					if (GroupBegin(1570, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{
						AddCheckbox(1571, BFH_LEFT, 0, 10, "");
						AddStaticText(1572, BFH_LEFT, 0, 10, "DWA Compression", 0);
						AddEditSlider(1573, BFH_SCALEFIT, 100, 10);
						GroupEnd();
					}
					// JPEG Compression
					if (GroupBegin(1580, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{
						AddCheckbox(1581, BFH_LEFT, 0, 10, "");
						AddStaticText(1582, BFH_LEFT, 0, 10, "JPEG Compression", 0);
						AddEditSlider(1583, BFH_SCALEFIT, 100, 10);
						GroupEnd();
					}
					// Storage
					if (GroupBegin(1590, BFH_SCALEFIT | BFV_SCALEFIT, 3, 0, "", 0, 0, 0))
					{
						AddCheckbox(1591, BFH_LEFT, 0, 10, "");
						AddStaticText(1592, BFH_LEFT, 0, 10, "Storage", 0);
						AddComboBox(1593, BFH_SCALEFIT, 100, 10, false);
						AddChild(1593, 0, "Scanline");
						AddChild(1593, 1, "Tiled");
						GroupEnd();
					}
					GroupEnd();
				}
				GroupEnd();
			}


			GroupEnd();
		}
		GroupEnd();
		AddButton(2000, BFH_SCALEFIT | BFV_SCALEFIT, 0, 20, "Ok");
	}
	return res;
}

Bool Ui::InitValues()
{
	// Vray
	SetBool(1101, vrayOptions.ConsiderForAA);
	SetBool(1102, vrayOptions.AffectMatteObject);

	// Octane
	SetBool(1201, octaneOptions.UseObjectId);
	SetBool(1202, octaneOptions.UseLayerID);

	// Redshift
	SetBool(1301, redshiftOptions.ReflectRefractIds);
	
	// Multipass Output
	SetBool(1411, redshiftOptions.UseMpEnable);
	SetBool(1413, redshiftOptions.MpEnable);
	SetBool(1421, redshiftOptions.UseMpBitPersChannels);
	SetInt32(1423, redshiftOptions.MpBitPersChannels);

	// File Output
	SetBool(1511, redshiftOptions.UseFoEnable); // Enable
	SetBool(1513, redshiftOptions.FoEnable);
	SetBool(1521, redshiftOptions.UseFoPath); // Path
	SetString(1523, redshiftOptions.FoPath);
	SetBool(1531, redshiftOptions.UseFoDataType); // Data Type
	SetInt32(1533, redshiftOptions.FoDataType);
	SetBool(1541, redshiftOptions.UseFoFormat); // Format
	SetInt32(1543, redshiftOptions.FoFormat);
	SetBool(1551, redshiftOptions.UseFoBitPersChannels); // Bits
	SetInt32(1553, redshiftOptions.FoBitPersChannels);
	SetBool(1561, redshiftOptions.UseFoCompression); // Compression
	SetInt32(1563, redshiftOptions.FoCompression);
	SetBool(1571, redshiftOptions.UseFoDwaCompression); // DWA Compression
	SetFloat(1573, redshiftOptions.FoDwaCompression);
	SetBool(1581, redshiftOptions.UseFoJpegQuality); // JPEG Compression
	SetInt32(1583, redshiftOptions.FoJpegQuality);
	SetBool(1591, redshiftOptions.UseFoStorage); // Storage
	SetInt32(1593, redshiftOptions.FoStorage);

	SetActiveState();
	SetHideState();
	SetCombo(false);

	return true;
}

Bool Ui::Command(Int32 id, const BaseContainer& msg)
{
	switch (id)
	{
		// Button Ok
		case 2000:
		{
			SetOptionsFromUi();
			_optIO.WriteOptionData(octaneOptions, vrayOptions, redshiftOptions);
			Close();
			CallCommand(1039895);
		}

		// Use checkbox
		case 1411: case 1421:case 1511: case 1521: case 1531: case 1541: case 1551: case 1561: case 1571: case 1581: case 1591:
		{
			SetActiveState();
			break;
		}

		// Multi Pass Folding
		case 1400:
		{
			_quickTabRedshiftMPState = !_quickTabRedshiftMPState;
			_quickTabRedshiftMP->Select(0, _quickTabRedshiftMPState);
			HideElement(1401, _quickTabRedshiftMPState);
			LayoutChanged(1300);
			break;
		}
			// File Output Folding
		case 1500:
		{
			// File Output
			_quickTabRedshiftFOState = !_quickTabRedshiftFOState;
			_quickTabRedshiftFO->Select(0, _quickTabRedshiftFOState);
			HideElement(1501, _quickTabRedshiftFOState);
			LayoutChanged(1300);
			break;
		}
		// Format
		case 1543:
		{
			SetHideState();
			SetCombo(true);
			break;
		}
		default:
			break;
	}
	return true;
}

void Ui::SetOptionsFromUi()
{
	Bool Buffer;
	Int32 BufferInt;
	Float BufferFloat;

	// Vray
	GetBool(1101, vrayOptions.ConsiderForAA);
	GetBool(1102, vrayOptions.AffectMatteObject);

	// Octane
	GetBool(1201, octaneOptions.UseObjectId);
	GetBool(1202, octaneOptions.UseLayerID);

	// Redshift
	GetBool(1301, redshiftOptions.ReflectRefractIds);

	// Multipass Output
	GetBool(1411, redshiftOptions.UseMpEnable);
	GetBool(1413, redshiftOptions.MpEnable);
	GetBool(1421, redshiftOptions.UseMpBitPersChannels);
	GetInt32(1423, BufferInt);
	redshiftOptions.MpBitPersChannels = BitPersChannels(BufferInt);

	// File Output
	GetBool(1511, redshiftOptions.UseFoEnable); // Enable
	GetBool(1513, redshiftOptions.FoEnable);

	GetBool(1521, redshiftOptions.UseFoPath); // Path
	GetString(1523, redshiftOptions.FoPath);

	GetBool(1531, redshiftOptions.UseFoDataType); // Data Type
	GetInt32(1533, BufferInt);
	redshiftOptions.FoDataType = FoDataType(BufferInt);

	GetBool(1541, redshiftOptions.UseFoFormat); // Format
	GetInt32(1543, BufferInt); 
	redshiftOptions.FoFormat = FoFormat(BufferInt);

	GetBool(1551, redshiftOptions.UseFoBitPersChannels); // Bits
	GetInt32(1553, BufferInt);
	redshiftOptions.FoBitPersChannels = BitPersChannels(BufferInt);

	GetBool(1561, redshiftOptions.UseFoCompression); // Compression
	GetInt32(1563, BufferInt);
	redshiftOptions.FoCompression = FoCompression(BufferInt);

	GetBool(1571, redshiftOptions.UseFoDwaCompression); // DWA Compression
	GetFloat(1573, BufferFloat);
	redshiftOptions.FoDwaCompression = Float32(BufferFloat);

	GetBool(1581, redshiftOptions.UseFoJpegQuality); // JPEG Compression
	GetInt32(1583, redshiftOptions.FoJpegQuality);

	GetBool(1591, redshiftOptions.UseFoStorage); // Storage
	GetInt32(1593, BufferInt);
	redshiftOptions.FoStorage = FoStorage(BufferInt);
}

void Ui::SetCombo(Bool change)
{
	Int32 format;
	Int32 valueBits;
	Int32 valueCompression;

	GetInt32(1543, format);
	GetInt32(1553, valueBits);
	GetInt32(1563, valueCompression);
	switch (format)
	{
		case exr:
		{
			// Bits
			FreeChildren(1553);
			AddChild(1553, 3, "Half Float (16 Bits)");
			AddChild(1553, 4, "Float (32 Bits)");
			if (change)
				SetInt32(1553, 3);
			else
				SetInt32(1553, valueBits);

			// Compression
			FreeChildren(1563);
			AddChild(1563, 0, "None");
			AddChild(1563, 1, "Default");
			AddChild(1563, 200, "RLE");
			AddChild(1563, 201, "ZIP");
			AddChild(1563, 202, "ZIPS");
			AddChild(1563, 203, "PIZ");
			AddChild(1563, 204, "PXR24");
			AddChild(1563, 205, "B44");
			AddChild(1563, 206, "DWAA");
			AddChild(1563, 207, "DWBB");
			if (change)
				SetInt32(1563, 1);
			else
				SetInt32(1563, valueCompression);

			break;
		}
		case tiff:
		{
			// Bits
			FreeChildren(1553);
			AddChild(1553, 0, "8 Bits");
			AddChild(1553, 1, "16 Bits");
			AddChild(1553, 4, "Float (32 Bits)");
			if (change)
				SetInt32(1553, 0);
			else
				SetInt32(1553, valueBits);

			// Compression
			FreeChildren(1563);
			AddChild(1563, 0, "None");
			AddChild(1563, 1, "Default");
			AddChild(1563, 100, "LZW");
			AddChild(1563, 101, "CCITT RLE");
			AddChild(1563, 102, "ZIP");
			AddChild(1563, 103, "PackBits");
			if (change)
				SetInt32(1563, 1);
			else
				SetInt32(1563, valueCompression);

			break;
		}
		case png:
		{
			// Bits
			FreeChildren(1553);
			AddChild(1553, 0, "8 Bits");
			AddChild(1553, 1, "16 Bits");
			if (change)
				SetInt32(1553, 0);
			else
				SetInt32(1553, valueBits);
			break;
		}
		default:
			break;
	}
}

void Ui::SetHideState()
{
	Int32 format;
	GetInt32(1543, format);
	switch (format)
	{
		case exr:
		{
			HideElement(GadgetPtr(1550), false);
			HideElement(GadgetPtr(1560), false);
			HideElement(GadgetPtr(1570), false);
			HideElement(GadgetPtr(1580), true);
			HideElement(GadgetPtr(1590), false);
			break;
		}
		case tiff:
		{
			HideElement(GadgetPtr(1550), false);
			HideElement(GadgetPtr(1560), false);
			HideElement(GadgetPtr(1570), true);
			HideElement(GadgetPtr(1580), true);
			HideElement(GadgetPtr(1590), false);
			break;
		}
		case png:
		{
			HideElement(GadgetPtr(1550), false);
			HideElement(GadgetPtr(1560), true);
			HideElement(GadgetPtr(1570), true);
			HideElement(GadgetPtr(1580), true);
			HideElement(GadgetPtr(1590), true);
			break;
		}
		case targa:
		{
			HideElement(GadgetPtr(1550), true);
			HideElement(GadgetPtr(1560), true);
			HideElement(GadgetPtr(1570), true);
			HideElement(GadgetPtr(1580), true);
			HideElement(GadgetPtr(1590), true);
			break;
		}
		case jpeg:
		{
			HideElement(GadgetPtr(1550), true);
			HideElement(GadgetPtr(1560), true);
			HideElement(GadgetPtr(1570), true);
			HideElement(GadgetPtr(1580), false);
			HideElement(GadgetPtr(1590), true);
			break;
		}
		default:
			break;
	}

	LayoutChanged(1401);
	LayoutChanged(1501);
	LayoutChanged(1550);
	LayoutChanged(1560);
	LayoutChanged(1570);
	LayoutChanged(1580);
	LayoutChanged(1590);
}

void Ui::SetActiveState()
{
	maxon::BaseArray<Int32> listIdToCheck;
	listIdToCheck.Append(1411);
	listIdToCheck.Append(1421);
	listIdToCheck.Append(1511);
	listIdToCheck.Append(1521);
	listIdToCheck.Append(1531);
	listIdToCheck.Append(1541);
	listIdToCheck.Append(1551);
	listIdToCheck.Append(1561);
	listIdToCheck.Append(1571);
	listIdToCheck.Append(1581);
	listIdToCheck.Append(1591);

	Bool value;
	for (Int32 i = 0; i < listIdToCheck.GetCount(); i++)
	{
		GetBool(listIdToCheck[i], value);
		Enable(listIdToCheck[i] + 1, value);
		Enable(listIdToCheck[i] + 2, value);
	}
}

