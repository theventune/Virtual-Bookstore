#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_TITRE 100
#define TAILLE_NOM 50

typedef struct Livre {
    int id;
    char titre[TAILLE_TITRE];
    char auteur[TAILLE_NOM];
    int disponible;
    int idEtudiant;
    struct Livre* suivant;
} Livre;

typedef struct Etudiant {
    int id;
    char nom[TAILLE_NOM];
    char prenom[TAILLE_NOM];
    struct Etudiant* suivant;
} Etudiant;

Livre* bibliotheque = NULL;
Etudiant* listeEtudiants = NULL;
int compteurIdLivre = 1;
int compteurIdEtudiant = 1;


Etudiant* trouverEtudiant(int id);
void afficherLivres();


void ajouterLivre() {
    Livre* nouveauLivre = (Livre*)malloc(sizeof(Livre));
    if (!nouveauLivre) {
        printf("\nErreur d'allocation de mémoire.\n");
        return;
    }

    nouveauLivre->id = compteurIdLivre++;
    printf("\nEntrez le titre du livre : ");
    getchar();
    fgets(nouveauLivre->titre, TAILLE_TITRE, stdin);
    nouveauLivre->titre[strcspn(nouveauLivre->titre, "\n")] = 0;
    printf("Entrez le nom de l'auteur : ");
    fgets(nouveauLivre->auteur, TAILLE_NOM, stdin);
    nouveauLivre->auteur[strcspn(nouveauLivre->auteur, "\n")] = 0;
    nouveauLivre->disponible = 1;
    nouveauLivre->idEtudiant = 0;
    nouveauLivre->suivant = bibliotheque;
    bibliotheque = nouveauLivre;

    printf("\nLivre ajouté avec succès !\n");
}

void afficherLivres() {
    if (!bibliotheque) {
        printf("\nAucun livre dans la bibliothèque.\n");
        return;
    }

    printf("\nListe des livres dans la bibliothèque :\n");
    Livre* courant = bibliotheque;
    while (courant) {
        printf("ID: %d, Titre: %s, Auteur: %s, Disponible: %s",
               courant->id,
               courant->titre,
               courant->auteur,
               courant->disponible ? "Oui" : "Non");


        if (!courant->disponible) {
            Etudiant* etudiant = trouverEtudiant(courant->idEtudiant);
            if (etudiant) {
                printf(", Emprunté par : ID: %d, Nom: %s, Prénom: %s",
                       etudiant->id,
                       etudiant->nom,
                       etudiant->prenom);
            } else {
                printf(", Emprunté par un étudiant inconnu (ID: %d)", courant->idEtudiant);
            }
        }
        printf("\n");
        courant = courant->suivant;
    }
}

Etudiant* trouverEtudiant(int id) {
    Etudiant* courant = listeEtudiants;
    while (courant) {
        if (courant->id == id) {
            return courant;
        }
        courant = courant->suivant;
    }
    return NULL;
}

void emprunterLivre() {
    int idLivre, idEtudiant;
    printf("\nEntrez l'ID du livre à emprunter : ");
    scanf("%d", &idLivre);

    Livre* courantLivre = bibliotheque;
    while (courantLivre) {
        if (courantLivre->id == idLivre) {
            if (!courantLivre->disponible) {
                printf("\nCe livre est déjà emprunté.\n");
                return;
            }
            printf("\nEntrez l'ID de l'étudiant : ");
            scanf("%d", &idEtudiant);

            if (!trouverEtudiant(idEtudiant)) {
                printf("\nÉtudiant introuvable.\n");
                return;
            }

            courantLivre->disponible = 0;
            courantLivre->idEtudiant = idEtudiant;
            printf("\nVous avez emprunté le livre : %s\n", courantLivre->titre);
            return;
        }
        courantLivre = courantLivre->suivant;
    }

    printf("\nID de livre invalide.\n");
}

void retournerLivre() {
    int id;
    printf("\nEntrez l'ID du livre à retourner : ");
    scanf("%d", &id);

    Livre* courant = bibliotheque;
    while (courant) {
        if (courant->id == id) {
            if (courant->disponible) {
                printf("\nCe livre est déjà disponible dans la bibliothèque.\n");
                return;
            }
            courant->disponible = 1;
            courant->idEtudiant = 0;
            printf("\nVous avez retourné le livre : %s\n", courant->titre);
            return;
        }
        courant = courant->suivant;
    }

    printf("\nID invalide.\n");
}

void supprimerLivre() {
    int id;
    printf("\nEntrez l'ID du livre à supprimer : ");
    scanf("%d", &id);

    Livre* courant = bibliotheque;
    Livre* precedent = NULL;

    while (courant) {
        if (courant->id == id) {
            if (precedent) {
                precedent->suivant = courant->suivant;
            } else {
                bibliotheque = courant->suivant;
            }
            free(courant);
            printf("\nLe livre a été supprimé avec succès.\n");
            return;
        }
        precedent = courant;
        courant = courant->suivant;
    }

    printf("\nID invalide.\n");
}

void ajouterEtudiant() {
    Etudiant* nouvelEtudiant = (Etudiant*)malloc(sizeof(Etudiant));
    if (!nouvelEtudiant) {
        printf("\nErreur d'allocation de mémoire.\n");
        return;
    }

    nouvelEtudiant->id = compteurIdEtudiant++;
    printf("\nEntrez le nom de l'étudiant : ");
    getchar();
    fgets(nouvelEtudiant->nom, TAILLE_NOM, stdin);
    nouvelEtudiant->nom[strcspn(nouvelEtudiant->nom, "\n")] = 0;
    printf("Entrez le prénom de l'étudiant : ");
    fgets(nouvelEtudiant->prenom, TAILLE_NOM, stdin);
    nouvelEtudiant->prenom[strcspn(nouvelEtudiant->prenom, "\n")] = 0;
    nouvelEtudiant->suivant = listeEtudiants;
    listeEtudiants = nouvelEtudiant;

    printf("\nÉtudiant ajouté avec succès !\n");
}

void afficherEtudiants() {
    if (!listeEtudiants) {
        printf("\nAucun étudiant enregistré.\n");
        return;
    }

    printf("\nListe des étudiants :\n");
    Etudiant* courant = listeEtudiants;
    while (courant) {
        printf("ID: %d, Nom: %s, Prénom: %s\n",
               courant->id,
               courant->nom,
               courant->prenom);
        courant = courant->suivant;
    }
}

void rechercherEtudiant() {
    char nom[TAILLE_NOM];
    char prenom[TAILLE_NOM];
    int trouve = 0;

    printf("\nEntrez le nom de l'étudiant : ");
    scanf("%s", nom);
    printf("Entrez le prénom de l'étudiant : ");
    scanf("%s", prenom);

    Etudiant* courant = listeEtudiants;

    printf("\n=== Résultat de la recherche ===\n");
    while (courant) {
        if (strcmp(courant->nom, nom) == 0 && strcmp(courant->prenom, prenom) == 0) {
            printf("ID : %d, Nom : %s, Prénom : %s\n", courant->id, courant->nom, courant->prenom);
            trouve = 1;
        }
        courant = courant->suivant;
    }

    if (!trouve) {
        printf("Aucun étudiant trouvé avec le nom '%s' et le prénom '%s'.\n", nom, prenom);
    }
}

void afficherEtudiantParId() {
    int id;
    printf("\nEntrez l'ID de l'étudiant : ");
    scanf("%d", &id);

    Etudiant* etudiant = trouverEtudiant(id);

    if (etudiant) {
        printf("\n=== Résultat de la recherche ===\n");
        printf("ID : %d, Nom : %s, Prénom : %s\n", etudiant->id, etudiant->nom, etudiant->prenom);
    } else {
        printf("\nAucun étudiant trouvé avec l'ID %d.\n", id);
    }
}

void supprimerEtudiant() {
    int id;
    printf("\nEntrez l'ID de l'étudiant à supprimer : ");
    scanf("%d", &id);

    Livre* courantLivre = bibliotheque;
    while (courantLivre) {
        if (courantLivre->idEtudiant == id) {
            printf("\nImpossible de supprimer cet étudiant. Il a emprunté un ou plusieurs livres.\n");
            return;
        }
        courantLivre = courantLivre->suivant;
    }

    Etudiant* courant = listeEtudiants;
    Etudiant* precedent = NULL;

    while (courant) {
        if (courant->id == id) {
            if (precedent) {
                precedent->suivant = courant->suivant;
            } else {
                listeEtudiants = courant->suivant;
            }
            free(courant);
            printf("\nL'étudiant a été supprimé avec succès.\n");
            return;
        }
        precedent = courant;
        courant = courant->suivant;
    }

    printf("\nID invalide.\n");
}

void libererMemoire() {
    Livre* courantLivre = bibliotheque;
    while (courantLivre) {
        Livre* tempLivre = courantLivre;
        courantLivre = courantLivre->suivant;
        free(tempLivre);
    }

    Etudiant* courantEtudiant = listeEtudiants;
    while (courantEtudiant) {
        Etudiant* tempEtudiant = courantEtudiant;
        courantEtudiant = courantEtudiant->suivant;
        free(tempEtudiant);
    }
}

void menu() {
    printf("\n=== Menu Bibliothèque Virtuelle ===\n");
    printf("1. Ajouter un livre\n");
    printf("2. Afficher les livres\n");
    printf("3. Emprunter un livre\n");
    printf("4. Retourner un livre\n");
    printf("5. Supprimer un livre\n");
    printf("6. Ajouter un étudiant\n");
    printf("7. Afficher les étudiants\n");
    printf("8. Supprimer un étudiant\n");
    printf("9. Rechercher un étudiant par nom et prénom\n");
    printf("10. Rechercher un étudiant par ID\n");
    printf("0. Quitter\n");
    printf("===================================\n");
    printf("Entrez votre choix : ");
}

int main() {
    int choix;

    do {
        menu();
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                ajouterLivre();
                break;
            case 2:
                afficherLivres();
                break;
            case 3:
                emprunterLivre();
                break;
            case 4:
                retournerLivre();
                break;
            case 5:
                supprimerLivre();
                break;
            case 6:
                ajouterEtudiant();
                break;
            case 7:
                afficherEtudiants();
                break;
            case 8:
                supprimerEtudiant();
                break;
            case 9:
                rechercherEtudiant();
                break;
            case 10:
                afficherEtudiantParId();
                break;
            case 0:
                printf("\nAu revoir !\n");
                break;
            default:
                printf("\nChoix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 0);

    libererMemoire();
    return 0;
}
