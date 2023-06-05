using namespace std;
#include <windows.h>
#include <iostream>
#include "CelikApi.h"
#include <fstream>
#include <string>
#include<direct.h>
#include <iostream>
#include <sstream>

#pragma execution_character_set( "utf-8" )

class EIDReader {// The class

	public: string prezime;
	public: string imeRoditelja;
	public: string ime;
	public: string godinaRodjenja;
	public: string mestoRodjenja;
	public: string drzavaRodjenja;
	public: string brojLK;
	public: string jmbg;
	public: string datumIzdavanjaLK;
	public: string datumIstekaLK;
	public: BYTE* portrait;
	public: int portraitSize;
	public: string personalFolder;

	public: EIDReader(string a, string b, string c, string d, string e, string f, string g, string h, string i, string j, BYTE* k, int l){//Setter
		
		prezime = a;
		imeRoditelja = b;
		ime = c;
		godinaRodjenja = d;
		mestoRodjenja = e;
		drzavaRodjenja = f;
		brojLK = g;
		jmbg = h;
		datumIzdavanjaLK = i;
		datumIstekaLK = j;
		portrait = k;
		portraitSize = l;
		personalFolder = "C:/xampp/htdocs/EIDReader/public/" + g;

	}

	public: int SaveJSONToFile() {

		ofstream person(personalFolder + "/person.json", ofstream::trunc);
		person << '"';
		person << "{";
		person << "'prezime': " << "'" << prezime << "'" << ",";
		person << "'imeRoditelja': " << "'" << imeRoditelja << "'" << ",";
		person << "'ime': " << "'" << ime << "'" << ",";
		person << "'godinaRodjenja': " << "'" << godinaRodjenja << "'" << ",";
		person << "'mestoRodjenja': " << "'" << mestoRodjenja << "'" << ",";
		person << "'drzavaRodjenja': " << "'" << drzavaRodjenja << "'" << ",";
		person << "'brojLK': " << "'" << brojLK << "'" << ",";
		person << "'jmbg': " << "'" << jmbg << "'" << ",";
		person << "'datumIzdavanjaLK': " << "'" << datumIzdavanjaLK << "'" << ",";
		person << "'datumIstekaLK': " << "'" << datumIstekaLK << "'";
		person << "}";
		person << '"';
		person.close();

		return 0;

	}

	public: int SavePortrait() {

		string myFile = personalFolder + "/avatar.jpg";
		ifstream avatarFile(myFile);
		if (avatarFile) {

			avatarFile.close();

			cout << "File exists." << endl;
			system("del avatar.jpg");
			cout << "Deleting remnant file." << endl;
			cout << "New file is being created." << endl;

			fstream avatar;
			avatar.open(myFile, std::ios::out | std::ios::binary | std::ios::trunc);
			avatar.write(reinterpret_cast<char*>(portrait), portraitSize); // or with recent C++: reinterpret_cast<std::byte*> or (char*)
			avatar.close();

		}
		else {

			avatarFile.close();

			cout << "File doesn't exist." << endl;
			cout << "File is being created." << endl;

			fstream avatar;
			avatar.open(myFile, std::ios::out | std::ios::binary | std::ios::trunc);
			avatar.write(reinterpret_cast<char*>(portrait), portraitSize); // or with recent C++: reinterpret_cast<std::byte*> or (char*)
			avatar.close();

		}

		return 0;

	}

	public: int SavePortraitAsString() {
		/*This method creates an string of indexes from ascii table.
		All one is need to do on receiving end is to to do lookup 
		in ascii table and find appropriate characters, 
		create string from it and create an image from it.
		*/
		ostringstream os;
		for (int i = 0; i < portraitSize;i++) {

			if (i < portraitSize-1) {
				os << (int)portrait[i] << ",";
			}
			else {
				os << (int)portrait[i];
			}

		}

		string str(os.str());
		cout << "Image string start: " << endl;
		cout << str << endl;
		cout << "Image string end." << endl;

		return 0;

	}

	public: int makeFolder() {
		//Creating a new Directory
		cout << personalFolder << endl;
		_mkdir(personalFolder.c_str());
		return 0;
	}

	public: int publishJSON() {

		cout << "'" << "{" << '"' << "surname" << '"' << ": " << '"' << prezime << '"' << ", " << '"' << "parentGivenName" << '"' << ": " << '"' << imeRoditelja << '"' << ", " << '"' << "givenName" << '"' << ": " << '"' << ime << '"' << ", " << '"' << "dateOfBirth" << '"' << ": " << '"' << godinaRodjenja << '"' << ", " << '"' << "placeOfBirth" << '"' << ": " << '"' << mestoRodjenja << '"' << ", " << '"' << "stateOfBirth" << '"' << ": " << '"' << drzavaRodjenja << '"' << ", " << '"' << "docRegNo" << '"' << ": " << '"' << brojLK << '"' << ", " << '"' << "personalNumber" << '"' << ": " << '"' << jmbg << '"' << ", " << '"' << "issuingDate" << '"' << ": " << '"' << datumIzdavanjaLK << '"' << ", " << '"' << "expiryDate" << '"' << ": " << '"' << datumIstekaLK << '"' << "}" << "'" << endl;
		return 0;

	}

	~EIDReader(){
		cout << "Ending of program..."<< endl;
	}

};

int main() {

	SetConsoleOutputCP(65001);

	SCARD_READERSTATE   readerState; // stanje citaca
	BOOL bDone = FALSE; // da li je kraj
	SCARDCONTEXT hSC;
	SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hSC); // uspostavi kontekst

	// ovo ili fiksno stavis, ili dobijas preko SCardListReaders funkcije
	//readerState.szReader = L"CASTLES EZ100PU 0"; 
	//NOVO!!! npr. ovo bira prvi citac koji je izlistan u listi
	LPTSTR pmszReaders = NULL;
	readerState.dwCurrentState = SCARD_STATE_UNAWARE; // pocetno stanje je neodredjeno
	DWORD cch = SCARD_AUTOALLOCATE; //automatski ce alocirati memoriju kada je potrebno (mora eksplicitno da se oslobodi
	//ova funkcija ubaci imena citaca u nul-terminated niz, npr. "citac1\0citac2\0citac3\0"
	if (SCardListReaders(hSC, NULL, (LPTSTR)&pmszReaders, &cch) == SCARD_S_SUCCESS) {
		readerState.szReader = pmszReaders; //nama je ionako potreban samo prvi
		cout << "Insert a card into card reader: " << (char*)pmszReaders << " : " << endl;
	}
	else {
		cout << "There are no card readers!" << endl;
		return 1;
	}

	EID_DOCUMENT_DATA docData;
	EID_FIXED_PERSONAL_DATA personFixData;
	EID_VARIABLE_PERSONAL_DATA personVarData;
	EID_PORTRAIT personPortrait;

	bool ifReadIsOK = false;

	EidStartup(3);
	while (!bDone)
	{
		// sacekaj proveru statusa
		SCardGetStatusChange(hSC, INFINITE, &readerState, 1); //ceka jednu sekundu da bi procitao status citaca
		// dvEventState je novo stanje, dvCurrentState je trenutno (staro) stanje
		// proveri status citaca
		if (readerState.dwEventState & SCARD_STATE_PRESENT) { // ako je novo stanje KARTICA_IN
			if ((readerState.dwCurrentState & SCARD_STATE_EMPTY) || // i ako je staro stanje KARTICA_OUT
				(readerState.dwCurrentState == SCARD_STATE_UNAWARE)) { // ili neodredjeno (samo za pocetni slucaj)
				//kartica je ubacena
				cout << "Card is in." << endl;
				EidBeginRead("");
				
				cout << "Card is being read." << endl;

				ZeroMemory(&docData, sizeof(EID_DOCUMENT_DATA)); // popuni sve nulama
				EidReadDocumentData(&docData);

				ZeroMemory(&personFixData, sizeof(EID_FIXED_PERSONAL_DATA)); // popuni sve nulama
				EidReadFixedPersonalData(&personFixData);

				ZeroMemory(&personVarData, sizeof(EID_VARIABLE_PERSONAL_DATA)); // popuni sve nulama
				EidReadVariablePersonalData(&personVarData);

				ZeroMemory(&personPortrait, sizeof(EID_PORTRAIT)); // popuni sve nulama
				EidReadPortrait(&personPortrait);

				ifReadIsOK = true;

				cout << "Card is done reading." << endl;
				EidEndRead();

			}
		}
		else if (readerState.dwEventState & SCARD_STATE_EMPTY) { // ako je novo stanje KARTICA_OUT
			if (readerState.dwCurrentState & SCARD_STATE_PRESENT) { // i ako je staro stanje KARTICA_IN
				// kartica je izvadjena
				cout << "Card is out." << endl;
			}
		}
		// sacuvaj novu vrednost stanja
		readerState.dwCurrentState = readerState.dwEventState;
		bDone = TRUE;

	} // glavna petlja while(!bDone)

	if (ifReadIsOK) {

		//Constructor klase za snimanje u fajlove.
		EIDReader Card(
			personFixData.surname,			//prezime
			personFixData.parentGivenName,	//imeRoditelja
			personFixData.givenName,		//ime 
			personFixData.dateOfBirth,		//godinaRodjenja
			personFixData.placeOfBirth,		//mestoRodjenja
			personFixData.stateOfBirth,		//drzavaRodjenja
			docData.docRegNo,				//brojLK
			personFixData.personalNumber,	//jmbg
			docData.issuingDate,			//datumIzdavanjaLK
			docData.expiryDate,				//datumIstekaLK
			personPortrait.portrait,		//Slika
			personPortrait.portraitSize		//Velicina slike potrebna za prebacivanje u fajl.
		);

		Card.makeFolder();
		//Card.SaveJSONToFile(); 
		Card.SavePortrait();
		Card.publishJSON(); 
		//Card.SavePortraitAsString();

	}
	else {
		cout << "Card isn't present!" << endl;
	}

	if(pmszReaders != NULL){
		SCardFreeMemory(hSC, pmszReaders);
	}
	EidCleanup();
	SCardReleaseContext(hSC);
	
	return 0;

}