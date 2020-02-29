#include "Combat.h"
#include "Affichage.h"
#include "Experiences.h"
#include <iostream>
#include "Aleatoire.h"

Combat::Combat(Equipes  & Joueur, Equipes  & Ia,Zones & Z,Animaux & A,Orbes & O) : _joueur{Joueur}, _ia{Ia},_tour{0}
{
	int somme = 0;
	int max = INT_MIN;
	int nombrePersonnages = _joueur.taille() + _ia.taille();
	std::cout << nombrePersonnages << " ";
	int xp;
	xp = _ia.xpDonner();
	std::vector<int> nombreTourJoueur(_joueur.taille());
	std::vector<int> nombreTourIa(_ia.taille());
	for (int i = 0;i < _joueur.taille();i++) {
		somme += _joueur.perso(i)->vitesse();
	}
	for (int i = 0;i < _ia.taille();i++) {
		somme += _ia.perso(i)->vitesse();
	}
	int moyenne = somme / nombrePersonnages;
	std::cout << moyenne << " ";
	for (int i = 0;i < _joueur.taille();i++) {
		nombreTourJoueur[i] = ceil((_joueur.perso(i)->vitesse()*1.0) / (moyenne*1.0));
		if (nombreTourJoueur[i] > max) {
			max = nombreTourJoueur[i];
		}
	}
	
	for (int i = 0;i < _ia.taille();i++) {
		nombreTourIa[i] = ceil((_ia.perso(i)->vitesse() *1.0) / (moyenne*1.0));
		if (nombreTourIa[i] > max) {
			max = nombreTourIa[i];
		}
	}
	std::cout << max << " ";
	for (int j = max;j > 0;j--) {
		for (int i = 0;i < _joueur.taille();i++) {
			if (nombreTourJoueur[i] >= j) {
				_quiJoue.push_back(_joueur.perso(i));
			}
		}
		for (int i = 0;i < _ia.taille();i++) {
			if (nombreTourIa[i] >= j) {
				_quiJoue.push_back(_ia.perso(i));
			}
		}
	}
	Affichage().dessinerDeuxEquipes(_joueur, _ia);
	int nbFoisJouer = 0;
	int nbJouerPourAugmenterTour = nombrePersonnages / 2;
	Affichage().affichageTexte(5, 5, "Tour : " + std::to_string(_tour + 1));
	while (_joueur.estEnVie() && _ia.estEnVie()) {
		for (int i = 0;i < _quiJoue.size();i++) {
			if ((_joueur.estEnVie() && _ia.estEnVie())) {
					if (_quiJoue[i]->estEnVie()) {
						nbFoisJouer++;
						nombrePersonnages = _joueur.nbEnVie() + _ia.nbEnVie();
						nbJouerPourAugmenterTour = nombrePersonnages / 2;
						if (nbFoisJouer%nbJouerPourAugmenterTour == 0) {
							_tour++;
							Affichage().affichageTexte(5, 3, "Tour : " + std::to_string(_tour+1));
							for (int i = 0; i < _joueur.taille(); i++) {
								if (_joueur[i]->estEnVie()) {
									_joueur[i]->passif(_tour);
								}
							}
							for (int i = 0; i < _ia.taille(); i++) {
								if (_ia[i]->estEnVie()) {
									_ia[i]->passif(_tour);
								}
							}
						}
						_quiJoue[i]->attaqueEnnemis();
					}
			}
		}
	}
	if (_joueur.estEnVie()) {
		Z.niveauBattu();
		Experiences E;
		
		_joueur.ajouterExperience(xp, E);
		tirageRecompenses(Z, A, O);
	}
	Affichage().dessinerDeuxEquipes(_joueur, _ia);
	std::cout << "Meilleur DPS : " << _joueur.meilleurDegats()->nom() << " " << _joueur.meilleurDegats()->stats().degatsProvoquer() << " degats." << std::endl;
	std::cout << "Meilleur TANK : " << _joueur.meilleurTank()->nom() << " " << _joueur.meilleurTank()->stats().degatsRecu() << " degats." << std::endl;
	std::cout << "Meilleur Soigneur : " << _joueur.meilleurSoigneur()->nom() << " " << _joueur.meilleurSoigneur()->stats().soinsDonner() << " vie." << std::endl;
	std::cout << "Meilleur Bouclier : " << _joueur.meilleurBouclier()->nom() << " " << _joueur.meilleurBouclier()->stats().bouclierDonner() << " bouclier." << std::endl;

	std::cout << "Meilleur augmentation de vie force : " << _joueur.meilleurAugmentationForce()->nom() << " " << _joueur.meilleurAugmentationForce()->stats().augmentationForce() << " points." << std::endl;
	std::cout << "Meilleur augmentation de vie maximum : " << _joueur.meilleurAugmentationVieMax()->nom() << " " << _joueur.meilleurAugmentationVieMax()->stats().augmentationVieMax() << " PV." << std::endl;
	std::cout << "Meilleur augmentation de reduction de degats : " << _joueur.meilleurAugmentationReduction()->nom() << " " << _joueur.meilleurAugmentationReduction()->stats().augmentationReduction() << " %." << std::endl;
	std::cout << "Meilleur augmentation de chance de coup habile : " << _joueur.meilleurAugmentationHabileter()->nom() << " " << _joueur.meilleurAugmentationHabileter()->stats().augmentationChanceHabileter() << " %." << std::endl;
	std::cout << "Meilleur augmentation de degats critique : " << _joueur.meilleurAugmentationDegatsCritiques()->nom() << " " << _joueur.meilleurAugmentationDegatsCritiques()->stats().augmentationDegatsCritique() << " %." << std::endl;
	std::cout << "Meilleur augmentation de chance de coup critique : " << _joueur.meilleurAugmentationChanceCritiques()->nom() << " " << _joueur.meilleurAugmentationChanceCritiques()->stats().augmentationCoupCritiques() << " %." << std::endl;
	std::cout << "Meilleur augmentation de chance de double attaques : " << _joueur.meilleurAugmentationChanceDoubleAttaque()->nom() << " " << _joueur.meilleurAugmentationChanceDoubleAttaque()->stats().augmentationChanceDoubleAttaque() << " %." << std::endl;
	std::cout << "Combat finis" << std::endl;
}

void Combat::tirageRecompenses(Zones Z,Animaux A,Orbes O) {
	int indiceJoueur;
	int nombreDutirage = _joueur.taille()*(1000000000-Z.niveauActuel()*Z.niveauActuel() * 10 + Z.niveauMax());
	int chanceTirage;
	//Pour chaque perso
	for (int i = 0; i < _joueur.taille(); i++) {
		//Pour chaque animal
		indiceJoueur = _joueur[i]->id();
		for (int j = 0; j < 9; j++) {
			//Pour chaque rarete animal
			for (int k = 1,chanceTirage=625; k <= 5; k++,chanceTirage/=5) {
				if (Aleatoire(0, nombreDutirage).entier() <= chanceTirage) {
					if (!A.animalDebloquer(indiceJoueur, j, k)) {
						A.deblocageAnimal(indiceJoueur, j, k, _joueur[i]->nom());
					}
				}
			}
		}
		//Pour chaque rareter d'orbe
		for (int j = 1, chanceTirage = 625; j <= 5; j++, chanceTirage /= 5) {
			if (Aleatoire(0, nombreDutirage).entier() <= chanceTirage) {
				if (!O.orbeDebloquer(indiceJoueur,j)) {
					O.deblocageOrbe(indiceJoueur, j, _joueur[i]->nom());
				}
			}
		}
	}
}
Combat::~Combat()
{
}



/*
void InitQuiJoue(){
	long long int somme = 0;
	for(int i=PREMIER;i<=CINQUIEME;i++) {
		somme+=PersoCarac[Equipe[TEAM1][i]][AGILITE];
		somme+=PersoCarac[Equipe[TEAM2][i]][AGILITE];
	}
	int Ratio[10];
	for(int i=0;i<=4;i++) {
		Ratio[i]=static_cast<int>(PersoCarac[Equipe[TEAM1][i]][AGILITE]/(1.0*somme/12));
		Ratio[i+5]=static_cast<int>(PersoCarac[Equipe[TEAM2][i]][AGILITE]/(1.0*somme/12));
	}

  for(double i=0.9;i>-0.1;i=i-0.1) {
	for(int j=0;j<=4;j++) {
		if(Ratio[j]>=i) {
			QuiJoue.push_back(Equipe[TEAM1][j]);
		  }
		if(Ratio[j+5]>=i) {
			QuiJoue.push_back(Equipe[TEAM2][j]);
		  }
	  }

  }
}
*/

