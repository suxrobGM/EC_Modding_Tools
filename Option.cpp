//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Option.h"
#include "Main.h"
#include "windows.h"
#include "IniFiles.hpp"
#include "fstream.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TOptionsForm *OptionsForm;
TIniFile *SettingsIni;

//---------------------------------------------------------------------------

__fastcall TOptionsForm::TOptionsForm(TComponent* Owner)
	: TForm(Owner)
{
    /**          *
	 *           *
	 * Win32 API *
	 *           *
	 *           */
	TCHAR szWinDir[_MAX_PATH]; //��������� ���� �� Win32 Api
	TCHAR szUserName [_MAX_FNAME]; //��� ������������ �� Win32 Api
	DWORD dwMaxFName = _MAX_FNAME;
	GetUserName(szUserName,&dwMaxFName); // Win32 Api
	GetSystemDirectory(szWinDir, _MAX_PATH); // Win32 Api


	UnicodeString UserName(szUserName);
	UnicodeString SysDir(szWinDir);
	SysDir = SysDir.SubString(0,2); //C:, D:, E: etc.

	//������������ ��������� �� settings.ini
	UnicodeString FileName = ProgrammPath + "settings.ini"; //���� + ��� ini ����
	SettingsIni = new TIniFile(FileName); //������� ����� ini ����
	ifstream iniFile(FileName.w_str()); //��������� ������ �� ini ����

	if(iniFile) //��������� ���� ���������� ini ����, �� ��������� ��������� ������
	{
		try{
			DirectoryListBox1->Directory = SettingsIni->ReadString("General Settings", "Mod Directory", SysDir+"\\Users\\"+UserName+"\\Documents\\Paradox Interactive\\Hearts of Iron IV\\mod\\");
			SetDirEdit->Text = DirectoryListBox1->Directory;
			ModDirectory = SettingsIni->ReadString("General Settings", "Mod Directory", SysDir+"\\Users\\"+UserName+"\\Documents\\Paradox Interactive\\Hearts of Iron IV\\mod\\ec\\");

			UnicodeString Language = SettingsIni->ReadString("General Settings", "Language", "English");
			if(Language=="Russian")
			{
				ChangeLanguage_CB->ItemIndex = 0;
			}
			else if(Language=="English")
			{
                ChangeLanguage_CB->ItemIndex = 1;
            }

		}
		catch (EInOutError&){
			ShowMessage("Please set true directory of mod in the Settings or in the file settings.ini");
			DirectoryListBox1->Directory = SysDir+"\\Users\\"+UserName+"\\Documents\\";
		}
	}
	else{
		DirectoryListBox1->Directory = SysDir+"\\Users\\"+UserName+"\\Documents\\Paradox Interactive\\Hearts of Iron IV\\mod\\";
	}
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::SetDirButtonClick(TObject *Sender)
{
	SetDirEdit->Text = DirectoryListBox1->Directory;
	SettingsIni->WriteString("General Settings", "Mod Directory", SetDirEdit->Text);
	SettingsIni->UpdateFile(); //��� �����������, ����� ������ ����������� ���� ini
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::ChangeLanguage_CBChange(TObject *Sender)
{
	if(ChangeLanguage_CB->ItemIndex == 0)
	{
		 //ShowMessage("�������");
		 SettingsIni->WriteString("General Settings", "Language", "Russian");
		 SettingsIni->UpdateFile();

	}
	else if(ChangeLanguage_CB->ItemIndex == 1)
	{
		 //ShowMessage("English");
		 SettingsIni->WriteString("General Settings", "Language", "English");
		 SettingsIni->UpdateFile();
	}
}

//---------------------------------------------------------------------------

