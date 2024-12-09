#ifndef SETTINGS_H
#define SETTINGS_H

// Style : \033[?m

// Normal - 0
// Bold - 1
// Dark - 2
// Underline - 4

// Black Foreground - 30
// Red Foreground - 31
// Green Foreground - 32
// Yellow Foreground - 33
// Blue Foreground - 34
// Magenta Foreground - 35
// Cyan Foreground - 36
// White Foreground - 37

// Black Background - 40
// Red Background - 41
// Green Background - 42
// Yellow Background - 43
// Blue Background - 44
// Magenta Background - 45
// Cyan Background - 46
// White Background - 47

#define VERSION "beta 1.16"

#define HELP_MESSAGE \
    "\nLe Solution Generator est un outil permettant de generer des solutions Visual Studio.\n" \
    "Utilisation : SolutionGenerator.exe [Options]\n"\
    "\n"\
    "Options\n"\
    "+----------------------------------------------------------------------------------------------------------------+\n"\
    "|                                                                                                                |\n"\
    "|  \033[2;37m-create\033[0;36m <Nom_du_repos> <Nom_de_la_solution> \033[2m{Nom_du_projet} {-pch} {-vcpkg} {-window} {-lib}\033[0m                  |\n"\
    "|                                                                                                                |\n"\
    "|          Creer un projet avec le Solution Generator                                                            |\n"\
    "|          \033[36m<Nom_du_repos>\033[0m Le nom du repos GitHub dans lequel la solution va s'installer                          |\n"\
    "|          \033[36m<Nom_de_la_solution>\033[0m Le nom de la solution Visual Studio qui va etre cree                             |\n"\
    "|          \033[2;36m{Nom_du_projet}\033[0m Le nom du premier projet qui va etre cree (optionnel), par defaut <Nom_de_la_solution>|\n"\
    "|          \033[2;36m{-pch}\033[0m Active les header precompiles (pch) pour le projet (optionnel)                                 |\n"\
    "|          \033[2;36m{-vcpkg}\033[0m Active le gestionnaire de dependances vcpkg pour le projet (optionnel)                       |\n"\
    "|                                                                                                                |\n"\
    "|  \033[2;37m-make\033[0;36m <dossier_racine> \033[2m{-clean} {-noOpen}\033[0m                                                                     |\n"\
    "|                                                                                                                |\n"\
    "|          \033[36m<dossier_racine>\033[0m Generer la solution a partir du dossier racine                                       |\n"\
    "|          \033[2;36m{-clean}\033[0m Retire les fichiers existants (optionnel)                                                    |\n"\
    "|          \033[2;36m{-noOpen}\033[0m N'ouvre pas l'explorateur de fichier aprws le make                                          |\n"\
    "|                                                                                                                |\n"\
    "|  \033[2;37m-add_port\033[0;36m <Nom_du_projet> <Dependance> <Nom_du_repos>\033[0m                                                         |\n"\
    "|                                                                                                                |\n"\
    "|          Ajouter une dependance avec vcpkg                                                                     |\n"\
    "|          \033[36m<Nom_du_projet>\033[0m Le nom du projet sur lequel ajouter la dependance                                     |\n"\
    "|          \033[36m<Dependance>\033[0m Le nom de la dependance vcpkg (https://vcpkg.link/ports)                                 |\n"\
    "|          \033[36m<Nom_du_repos>\033[0m Le nom du repos GitHub ou se trouve le projet                                          |\n"\
    "|                                                                                                                |\n"\
    "|  \033[2;37m-add_project\033[0;36m <Nom_du_projet> <Nom_du_repos>\033[0m                                                                   |\n"\
    "|                                                                                                                |\n"\
    "|          Ajouter un projet a la solution Visual Studio                                                         |\n"\
    "|          \033[36m<Nom_du_projet>\033[0m Le nom du projet a ajouter a la solution Visual Studio                                |\n"\
    "|          \033[36m<Nom_du_repos>\033[0m Le nom du repos GitHub ou se trouve le projet                                          |\n"\
    "|                                                                                                                |\n"\
    "|  \033[2;37m-add_reference\033[0;36m <Nom_du_projet> <Reference> <Nom_du_repos>\033[0m                                                     |\n"\
    "|                                                                                                                |\n"\
    "|          Ajoute un projet en reference a un autre projet                                                       |\n"\
    "|          \033[36m<Nom_du_projet>\033[0m Le nom du projet auquel rajouter une reference                                        |\n"\
    "|          \033[36m<Reference>\033[0m Le nom du projet a rajouter en tant que reference                                         |\n"\
    "|          \033[36m<Nom_du_repos>\033[0m Le nom du repos GitHub ou se trouve le projet                                          |\n"\
    "|                                                                                                                |\n"\
    "|  \033[2;37m-help\033[0m                                                                                                         |\n"\
    "|         Montre ce message d'aide                                                                               |\n"\
    "|                                                                                                                |\n"\
    "+----------------------------------------------------------------------------------------------------------------+\n"

#define PCH_H \
    "#pragma once\n"

#define PCH_CPP \
    "#include \"pch.h\"\n"

#define VCPKG_JSON \
    "{\n"\
    "  \"dependencies\": [\n"\
    "  ]\n"\
    "}"

#define PRJ_FILE \
    "{\n"\
    "    \"solution_name\":\"\" \n"\
    "}"

#define PROJECT_JSON \
    "{\n"\
    R"(    "vcpkg": "false",)" \
    R"(    "pch": "false",)" \
    R"(    "window": "false",)" \
    R"(    "lib": "false")" \
    "}\n"\
    
#define MAIN \
    "int main()\n" \
    "{\n" \
    "    return 0;\n" \
    "}"

#define WINMAIN \
    "int WinMain()\n" \
    "{\n" \
    "    return 0;\n" \
    "}"

#endif
