#include "Fiona.h"
#include "Affichage.h"
#include "Lapin.h"
#include "Sanglier.h"	

Fiona::Fiona(Experiences E, Orbes O, Animaux A, Objets Obj) : Personnage(3, E, O, A, Obj, "Fiona", 3, 5, 2, 17, 17, -17, 7, 7, 7, 70) , _nbAnimaux{0} {}

void Fiona::attaqueEnnemis()
{
	int choix = choixAttaque();
	int DEGATS;
	int SOINS;
	
	switch (choix) {

	case 0:
		equipeEnnemi().plusProcheVivant()->ajouterReduction(-1);
		DEGATS = degats(0.4, 0.8);
		Affichage().dessinerTexte(nom() + " tire oreille");
		Attaque(DEGATS, equipeEnnemi().plusProcheVivant());
		if (attaqueDouble() && equipeEnnemi().estEnVie()) {
			DEGATS = degats(0.55, 0.95);
			Attaque(DEGATS, equipeEnnemi().plusProcheVivant());
		}
		ajouterMana(1);
		break;
	case 1:
		Affichage().dessinerTexte(nom() + " soins picher ");
		for (int i = 0; i < 7; i++) {
			while (ricoche()) {
				SOINS = soins(0.1, 0.25);
				soigner(SOINS, equipeAllier().aleatoireEnVie());
			}
		}
		if (attaqueDouble() ) {
			for (int i = 0; i < 7; i++) {
				while (ricoche()) {
					SOINS = soins(0.25, 0.5);
					soigner(SOINS, equipeAllier().aleatoireEnVie());
				}
			}
		}
		ajouterMana(+1);
		break;
	case 2:

		Affichage().dessinerTexte(nom() + " les 3 pichnettes ");
		for (int i = 1; i <= 3 && equipeEnnemi().estEnVie(); i++) {
			equipeEnnemi().plusProcheVivant()->ajouterReduction(-1);
			DEGATS = degats(0.2 * i , 0.4 *i);
			Attaque(DEGATS, equipeEnnemi().plusFort());
		}
		if (attaqueDouble() && equipeEnnemi().estEnVie()) {
			for (int i = 1; i <= 3 && equipeEnnemi().estEnVie(); i++) {
				equipeEnnemi().plusProcheVivant()->ajouterReduction(-1);
				DEGATS = degats(0.3 * i, 0.6 * i);
				Attaque(DEGATS, equipeEnnemi().plusFort());
			}
		}
		ajouterMana(-2);
		break;
	case 3:

	
		DEGATS = degats(0.7, 1.7);
		equipeEnnemi().plusProcheVivant()->ajouterReduction(-7);

		Affichage().dessinerTexte(nom() + " p�issance 17 ");
		Attaque(DEGATS, equipeEnnemi().plusProcheVivant());
		ajouterChanceHabileter(7);
		if (attaqueDouble() && equipeEnnemi().estEnVie()) {
			DEGATS = degats(1.17, 1.77);
			equipeEnnemi().plusProcheVivant()->ajouterReduction(-7);
			Attaque(DEGATS, equipeEnnemi().plusProcheVivant());
		}
		ajouterMana(-3);
		break;
	}
	for (int i = 0; i < equipeAllier().taille() && equipeEnnemi().estEnVie();i++) {
		if (equipeAllier()[i]->id() == 3 && equipeAllier()[i]->nom() != nom()) {
			equipeAllier()[i]->attaqueEnnemis();
		}
	}
}

void Fiona::passif(int tour)
{
	std::string nom;
	int Degats;
	if (((tour + 1) % 25) == 0) {
		if (equipeAllier().taille() < 10) {
			if (_nbAnimaux == 0) {
				nom = " Perle";
				equipeAllier().ajouterPerso(new Lapin(this->niveau(), nom, 0, 9, 5, 3));
			}
			else if (_nbAnimaux == 1) {
				nom = " Oxanne";
				equipeAllier().ajouterPerso(new Sanglier(this->niveau(), nom, 0, 9, 5, 3));
			}
			if (_nbAnimaux < 2) {
				Affichage().dessinerTexte(this->nom() + " adopte un animal ! ");
				
				equipeAllier()[equipeAllier().taille() - 1]->setEnnemis(equipeEnnemi());
				equipeAllier()[equipeAllier().taille() - 1]->setAllier(equipeAllier());
				Affichage().dessinerEquipeJoueur(equipeAllier());
				_nbAnimaux++;
			}
		}
	}
	if (((tour + 1) % 7) == 0) {
		ajouterCoupCritique(7);
		if (habile()) {
			ajouterDegatsCritique(14);
		}
	}
	if (((tour + 1) % 70) == 0) {
		ajouterDegatsCritique(17);
		for (size_t i = 0; i < 17; i++)
		{
			Degats = degats(0.17, 0.18 + 0.01 * i);
			Attaque(Degats, equipeEnnemi().plusFort());
		}
	}
}

void Fiona::passifDefensif()
{
	ajouterReduction(1);
	for (int i = 0; i < equipeAllier().taille()&&equipeEnnemi().estEnVie();i++) {
		if (equipeAllier()[i]->id() == 3 && equipeAllier()[i]->nom() != nom()) {
			equipeAllier()[i]->attaqueEnnemis();
			equipeAllier()[i]->attaqueEnnemis();
			equipeAllier()[i]->attaqueEnnemis();
		}
	}
}
