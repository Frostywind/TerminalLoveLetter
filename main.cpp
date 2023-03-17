
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <map>
#include <string>

#define clearShell() printf("\033[H\033[J")
#define redShell() printf("\u001b[31m")
#define greenShell() printf("\u001b[38;5;2m")
#define pinkBackShell() printf("\u001b[48;5;212m")
#define resetShell() printf("\u001b[0m")

const std::string cardRef[10] = {"Espionne", "Garde", "Prêtre", "Baron", "Servante", "Prince", "Chancelier", "Roi", "Comtesse", "Princesse"};
using cardNumber = unsigned char;

struct Deck {
    auto begin() {
        return cards + beginIndex;
    }

    auto end() {
        return cards + endIndex;
    }

    cardNumber draw() {
        cardNumber nextCard = static_cast<cardNumber>(-1);

        if (beginIndex != endIndex) {
            nextCard = cards[beginIndex];
            beginIndex++;
        } 

        return nextCard;
    }

    void insertBack(std::vector<cardNumber> &cards) {
        for (cardNumber card : cards) {
            cards[endIndex] = card;
            endIndex++;
        }
    }

    bool empty() const {
        return beginIndex == endIndex;
    }

private :
    int endIndex = 21;
    int beginIndex = 0;
    cardNumber cards[25] = {0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 9};
};

class Game {
public :
    Game() : gameContinue(true), deadPlayer(""), targetedPlayer("") {        
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deck.begin(), deck.end(), g);

        initPlayers();
    }

    ~Game() = default;

    void launchGame() {
        auto actualPlayer = players.begin();
        while(gameContinue) {
            // ActualPlayer draw one card
            std::cout << "\n\n\n\t   Tour de " << actualPlayer->first << '\n';
            
            auto servIt = std::find(servantPlayer.begin(), servantPlayer.end(), actualPlayer->first);
            if (servIt != servantPlayer.end()) {
                servantPlayer.erase(servIt);
            }

            cardNumber cardDraw = deck.draw();

            // ActualPlayer choose one card to play

            cardNumber cardPlayed;
            if (actualPlayer->second == 8 && (cardDraw == 5 || cardDraw == 7)) {
                std::cout << "Comtesse joué automatiquement" << '\n';
                actualPlayer->second = cardDraw;
                cardPlayed = 8;
                cardEffect(8, *(new std::string(actualPlayer->first)));
            } else if (cardDraw == 8 && (actualPlayer->second == 5 || actualPlayer->second == 7)) {
                std::cout << "Comtesse joué automatiquement" << '\n';
                cardPlayed = 8;
                cardEffect(8, *(new std::string(actualPlayer->first)));
            } else {
                std::cout << "\nVous avez pouvez jouer les cartes :";
                std::cout << "\n\n1. " << cardRef[actualPlayer->second];
                std::cout << "\n2. " << cardRef[cardDraw];
                std::cout << "\n\nLaquelle souhaitait vous jouer [1 ou 2] : ";
                int cardChoice;
                std::cin >> cardChoice;
                std::cout << '\n';
                if (cardChoice == 1) {
                    cardPlayed = actualPlayer->second;
                    actualPlayer->second = cardDraw;
                    cardEffect(cardPlayed, *(new std::string(actualPlayer->first)));
                } else {
                    cardPlayed = cardDraw;
                    cardEffect(cardPlayed, *(new std::string(actualPlayer->first)));
                }
            }
            
            // Is the game finish ?
            if (players.size() < 2 || deck.empty()) {
                gameContinue = false;
            } else {
                // End of turn
                auto ancientPlayer = actualPlayer;
                actualPlayer++;
                if (actualPlayer == players.end()) {
                    actualPlayer = players.begin();
                }
                summaryScreen(*(new std::string(ancientPlayer->first)), *(new std::string(actualPlayer->first)), cardPlayed);
            }
        }

        gameEnding();
    }

private :
    bool gameContinue;
    unsigned int playerNumber;
    std::string deadPlayer;
    std::string targetedPlayer;
    Deck deck;
    std::map<std::string, cardNumber> players;
    std::vector<std::string> spyPlayer;
    std::vector<std::string> servantPlayer;

    void titleScreen() {
        clearShell();
        pinkBackShell();
        greenShell();
        printf("\n\n\n\n*************************************************************************************");
        printf("\n                                                                                     ");
        printf("\n                                                                                     ");
        printf("\n                                                                                     ");                                       
        printf(
        "\n ___     _______ __   __ _______    ___     _______ _______ _______ _______ _______  "
        "\n|   |   |       |  | |  |       |  |   |   |       |       |       |       |    _  | "
        "\n|   |   |   _   |  |_|  |    ___|  |   |   |    ___|_     _|_     _|    ___|   | | | "
        "\n|   |   |  | |  |       |   |___   |   |   |   |___  |   |   |   | |   |___|   |_| | "
        "\n|   |___|  |_|  |       |    ___|  |   |___|    ___| |   |   |   | |    ___|       | "
        "\n|       |       ||     ||   |___   |       |   |___  |   |   |   | |   |___|   |\\  \\ "
        "\n|_______|_______| |___| |_______|  |_______|_______| |___|   |___| |_______|___| \\__|");
        printf("\n                                                                                     ");
        printf("\n                                                                                     ");
        printf("\n                                                                                     ");  
        printf("\n*************************************************************************************");
        printf("\n");
        resetShell();
        printf("\n\n\t\t\t     Appuyer sur Entrer");
        std::cin.ignore();
        clearShell();
    }

    void summaryScreen(std::string &ancientPlayer, std::string &actualPlayer, int cardPlayed) {
        std::cout << "\nAppuyer sur entrer pour continuer";
        std::cin.ignore();
        std::cin.ignore();
        clearShell();
        std::cout << "\n\n\nLe joueur " << ancientPlayer << " vient de jouer la carte " << cardRef[cardPlayed];
        if (targetedPlayer != "") {
            std::cout << " sur " << targetedPlayer;
            targetedPlayer = "";
        }
        if (deadPlayer != "") {
            std::cout << "\n\nLe joueur : " << deadPlayer << " est mort ce tour *Musique dramatique*";
            deadPlayer = "";
        }
        std::cout << "\n\nC'est maintenant au tour de " << actualPlayer;
        std::cout << "\n\n\nAppuyer sur entrer pour continuer";
        std::cin.ignore();
        clearShell();
    }

    void initPlayers() {
        titleScreen();

        do {
            std::cout << "\n\n\nCe jeu se joue de 2 à 6 joueurs";
            std::cout << "\n\nNombre de joueur : ";
            std::cin >> playerNumber;
        } while (playerNumber < 2 || playerNumber > 6);
        std::cout << '\n';

        std::string playerName;
        for (int i = 0; i < playerNumber; i++) {
            std::cout << "Nom du joueur " << i + 1 << " : ";
            std::cin >> playerName;
            players[playerName] = deck.draw();
        }
        clearShell();
    }

    void gameEnding() {
        if (players.size() < 2) {
            std::cout << "La.e joueuse.r " << players.begin()->first;
            if (!spyPlayer.empty()) {
                std::cout << " a doublement gagné !! Trop fort !" << '\n';
            } else {
                std::cout << " a gagné.e !" << '\n';
            }
        } else {
            unsigned int maxValue = 0;
            std::vector<std::string> victoriousPlayers(playerNumber);
            for(auto [name, value] : players) {
                std::cout << name << ' ' << (unsigned int)value << '\n';
                if (value > maxValue) {
                    victoriousPlayers.clear();
                    victoriousPlayers.push_back(name);
                    maxValue = value;
                } else if (value == maxValue) {
                    victoriousPlayers.push_back(name);
                }
            }
            std::cout << "Le.a.s gagnant.e.s sont : ";
            for(auto winner : victoriousPlayers) {
                std::cout << winner << " | ";
            }
            std::cout << '\n';
            if(spyPlayer.size() == 1) {
                std::cout << "La.e joueuse.r a gagné.e avec l'espionne" << '\n';
            }
        }
    }

    void spy(std::string &actualPlayer) {
        std::cout << "La lettre n'est qu'une couverture c'est celle d'un espion !" << '\n';
        if (std::find(spyPlayer.begin(), spyPlayer.end(), actualPlayer) == spyPlayer.end()) {
            spyPlayer.push_back(actualPlayer);
        }
    }

    void guard(std::string &actualPlayer) {
        std::cout << "C'est l'heure de la délation !!!" << '\n';

        std::string targetPlayer = choosePlayer(actualPlayer);

        if (targetPlayer != "") {
            unsigned int cardGuess;
            do {
                std::cout << "Essaye de deviner le numéro de sa carte : ";
                std::cin >> cardGuess;
            } while (cardGuess < 0 || cardGuess > 9 || cardGuess == 1);
            
            if (cardGuess == players.at(targetPlayer)) {
                killPlayer(targetPlayer);
            } else {
                std::cout << "\nC'est non" << '\n';
            }
        }
    }

    void priest(std::string &actualPlayer) {
        std::cout << "Le prêtre connait tous les potins" << '\n';

        std::string targetPlayer = choosePlayer(actualPlayer);
                
        if (targetPlayer != "") {
            std::cout << "Il possède la carte : " << cardRef[players.at(targetPlayer)] << '\n';
        }
    }

    void baron(std::string &actualPlayer) {
        std::cout << "Le Baron est là ça va saigner !! (Probablement)" << '\n';

        std::string targetPlayer = choosePlayer(actualPlayer);

        if (targetPlayer != "") {
            int baronResult = players.at(actualPlayer) - players.at(targetPlayer);
            if (baronResult > 0) {
                killPlayer(targetPlayer);
            } else if (baronResult < 0) {
                killPlayer(actualPlayer);
            } else {
                std::cout << "Rien ne se passe...";
            }
        }
    }

    void servant(std::string &actualPlayer) {
        std::cout << "Vous êtes protéger pour un tour par la servante [DAB]" << '\n';
        servantPlayer.push_back(actualPlayer);
    }

    void prince(std::string &actualPlayer) {
        std::cout << "Il manque pas de bel aire ce prince !" << '\n';

        std::string *playerName = new std::string("");
        std::string targetPlayer = choosePlayer(*playerName);

        if (players.at(targetPlayer) == 9) {
            killPlayer(targetPlayer);
        } else {
            if (players.at(targetPlayer) == 0) {
                std::cout << "ESPION" << '\n';
                spyPlayer.push_back(actualPlayer);
            }
            players[targetPlayer] = deck.draw();
        }
    }

    void chancellor(std::string &actualPlayer) {
        std::cout << "Le Conseiller prodigue de bon conseil" << '\n';

        cardNumber firstChoice = deck.draw();
        cardNumber secondChoice = deck.draw();
        std::vector<cardNumber> *choices = new std::vector<cardNumber>({firstChoice, secondChoice, players.at(actualPlayer)});
        
        std::cout << "\nVous avez les cartes :";
        std::cout << "\n\n1. " << cardRef[choices->at(0)];
        std::cout << "\n2. " << cardRef[choices->at(1)];
        std::cout << "\n3. " << cardRef[choices->at(2)];
        std::cout << "\n\nLaquelle souhaitait vous garder [1, 2 ou 3] : ";
        
        int cardPlayed;
        std::cin >> cardPlayed;
        players[actualPlayer] = choices->at(cardPlayed - 1);
        choices->erase(choices->begin() + cardPlayed - 1);


        std::cout << "\nVous avez les cartes :";
        std::cout << "\n\n1. " << cardRef[choices->at(0)];
        std::cout << "\n2. " << cardRef[choices->at(1)];
        std::cout << "\n\nQuelle carte souhaiter vous mettre tout en dessous du deck [1 ou 2] : ";
        
        int cardBelow;
        std::cin >> cardBelow;
        if (cardBelow - 1 == 0) std::swap(choices->at(0), choices->at(1));
        deck.insertBack(*choices);
    }

    void king(std::string &actualPlayer) {
        std::cout << "The king !" << '\n';
        std::string targetPlayer = choosePlayer(actualPlayer);                               

        if (targetPlayer != "") {
            cardNumber tempCard = players.at(actualPlayer);
            players[actualPlayer] = players.at(targetPlayer);
            players[targetPlayer] = tempCard;
        }
    }

    void countess(std::string &actualPlayer) {
        std::cout << "La comptesse ! (DOUBT)" << '\n';
    }

    void princess(std::string &actualPlayer) {
        std::cout << "La Princesse est partie !" << '\n';
        killPlayer(actualPlayer);
    }

    using funPtr = void(Game::*)(std::string&);
    void cardEffect(cardNumber cardPlayed, std::string &actualPlayer) {
        funPtr funPtrArray[10];
        funPtrArray[0] = &Game::spy;
        funPtrArray[1] = &Game::guard;
        funPtrArray[2] = &Game::priest;
        funPtrArray[3] = &Game::baron;
        funPtrArray[4] = &Game::servant;
        funPtrArray[5] = &Game::prince;
        funPtrArray[6] = &Game::chancellor;
        funPtrArray[7] = &Game::king;
        funPtrArray[8] = &Game::countess;
        funPtrArray[9] = &Game::princess;

        (*this.*funPtrArray[cardPlayed])(actualPlayer);
    }

    std::string choosePlayer(std::string &actualPlayer) {
        std::string playerName;
        if (actualPlayer != "" && players.size() - 1 == servantPlayer.size()) {
            std::cout << "Aucun.e joueur.se ne peut être ciblé.e" << '\n';
            playerName = "";
        } else {
            std::cout << "\nLes joueurs suivant peuvent être ciblée :";
            auto targetablePlayers = players.begin();
            while (targetablePlayers != players.end()) {
                if ((std::find(servantPlayer.begin(), servantPlayer.end(), targetablePlayers->first) == servantPlayer.end()) && 
                actualPlayer != targetablePlayers->first) {
                    std::cout << "\n- " << targetablePlayers->first;
                }
                targetablePlayers++;
            }
            std::cout << '\n';
            do {
                std::cout << "\nQuel.le joueuse.r voulez vous ciblé.e : ";
                std::cin >> playerName;
            } while (playerName == actualPlayer || 
                !(std::find(servantPlayer.begin(), servantPlayer.end(), playerName) == servantPlayer.end()) || 
                !players.contains(playerName));

            std::cout << '\n';
        }

        targetedPlayer = playerName;
        return playerName;
    }

    void killPlayer(std::string &playerName) {
        auto player = players.find(playerName);
        auto spyIt = std::find(spyPlayer.begin(), spyPlayer.end(), playerName);
        if (spyIt != spyPlayer.end()) {
            spyPlayer.erase(spyIt);
        }
        auto servIt = std::find(servantPlayer.begin(), servantPlayer.end(), playerName);
        if (servIt != servantPlayer.end()) {
            servantPlayer.erase(servIt);
        }
        deadPlayer = playerName;
        player = players.erase(player);
        std::cout << playerName << " est mort" << '\n';
    }
};

int main()
{
    Game game;
    game.launchGame();
    return 0;
}
