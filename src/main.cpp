/* @author:         Atish Patange
 * @e-mail:         patangeatish@simple-robot.de
 * @description:    Once upon a time there was a great Warrior on the banks of Ludwigshafen. He was living happily,
 *                  but oneday, the Orc the big and bad guy and his friend Dragon attacked Hero mercillesly. But, hero has
 *                  the weapon called "Keyboard X." The Keyboard X can enter Orc`s and Dragon`s brain and can alter their state.
 *                  With commands, he can break the teeth from Orc and fire up the fuel from Dragon. He just need to write a
 *                  simple command "Attack." The Hero fought back mercilessly, and killed both of them!
 * */

#include <stdio.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>

/* @class:          globalStat
 * @description:    statistics about game. It holds health of all characters and execute operations
 *                  of attack
 * */
class GlobalStat
{
public:
    GlobalStat()
    {
        // constructor
    }
    virtual ~GlobalStat()
    {
        // destructor
    }

    /*  @function:      attack()
     *  @description:   update global statistics
     *  @input:         a_attackee: chracter getting attacked
     *                  a_attacker: character attacking Hero
     *                  a_damage:   damage point
     * */
    void attack(const std::string& a_attackee, const std::string& a_attacker, const int& a_damage)
    {
	stateMutex.lock();
    	if(a_attackee == "Hero")
        {
            std::cout << "-> Hero attacked by " << a_attacker << std::endl;
            m_heroHealth -= a_damage;
        }
        else if(a_attackee == "Dragon")
        {
            std::cout << "-> Dragon attacked by " << a_attacker << std::endl;
            m_dragonFuel -= a_damage;
        }
        else if(a_attackee == "Orc")
        {
            std::cout << "-> Orc attacked by " << a_attacker << std::endl;
            m_orcTeeth -= a_damage;
        }
	stateMutex.unlock();
    }

    int& getHealthStatus(const std::string& a_character)
    {
        if(a_character == "Hero")
        {
            return m_heroHealth;
        }
        else if(a_character == "Dragon")
        {
            return m_dragonFuel;
        }
        else if(a_character == "Orc")
        {
            return m_orcTeeth;
        }
    }

    bool isGameContinue()
    {
        return (!m_isGameLost && !m_isGameWin);
    }

    void scoreBoard()
    {
        if (!m_isOrcDead)
            std::cout << "Orc`s health -> "       << getHealthStatus("Orc")    << std::endl;
        if (!m_isDragonDead)
            std::cout << "Dragon`s health -> "    << getHealthStatus("Dragon") << std::endl;
        if (!m_isHeroDead)
            std::cout << "Hero`s health -> "      << getHealthStatus("Hero")   << std::endl;
    }

    void updateGameState()
    {
        stateMutex.lock();
	if(getHealthStatus("Orc")  <= 0 && !m_isOrcDead)
        {
            m_isOrcDead = true;
            std::cout << "-> Orc is dead! Hurray! Thank you Ludwigshafen, for trusting me!" << std::endl;
        }

        if(getHealthStatus("Dragon") <= 0 && !m_isDragonDead)
        {
            m_isDragonDead = true;
            std::cout << "-> Dragon is dead! Hurray! We will have bash tonight!" << std::endl;
        }

        if(getHealthStatus("Hero") <= 0)
        {
            m_isHeroDead = true;
            std::cout << "-> I will come again! I deserve one more chance " << std::endl;
        }

        if(m_isOrcDead && m_isDragonDead)
        {
            m_isGameWin = true;
            std::cout << "#YOU WIN THE GAME! " << std::endl;
        }

        if(m_isHeroDead)
        {
            m_isGameLost = true;
            std::cout << "#YOU LOOSE THE GAME! " << std::endl;
        }
	stateMutex.unlock();
    }
private:
    std::mutex stateMutex;

    // medics should know the state of the health
    int m_heroHealth  = 40;
    int m_dragonFuel  = 20;
    int m_orcTeeth    = 7;

    // is war still continued
    bool m_isGameLost   = false;
    bool m_isGameWin    = false;

    // the death count of the war
    bool m_isHeroDead   = false;
    bool m_isOrcDead    = false;
    bool m_isDragonDead = false;
};

class HeroFromLudwigshafen
{
public:
    void operator()(GlobalStat* a_pntr)
    {
        while(a_pntr->isGameContinue())
        {
            std::string ipString = "";
            std::getline(std::cin, ipString);

            if (ipString == "Attack Orc")
            {
                // reduce Orcs 2 molecules
                a_pntr->attack("Orc", "Hero", 2);

            }
            else if(ipString == "Attack Dragon")
            {
                // destroy Dragons fuel tank by 2 litre
                a_pntr->attack("Dragon", "Hero", 2);
            }
            else
            {
                // do nothing
            }
            a_pntr->scoreBoard();
            a_pntr->updateGameState();
        }
    }
};

class OrcsUglyWorld
{
public:
    void operator()(GlobalStat* a_pntr)
    {
        std::cout << "#Orcs Ugly World started " << std::endl;
        while(a_pntr->isGameContinue())
        {
            // after 1500ms, attack hero for 1 damage
            a_pntr->attack("Hero", "Orc", 1);
            a_pntr->scoreBoard();
            a_pntr->updateGameState();
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
    }
};

class DragonsDirtyWorld
{
public:
    void operator()(GlobalStat* a_pntr)
    {
        std::cout << "#Dragons Ugly World started " << std::endl;
        while(a_pntr->isGameContinue())
        {
            // after 1500ms, attack hero for 3 damage
            a_pntr->attack("Hero", "Dragon", 3);
            a_pntr->scoreBoard();
            a_pntr->updateGameState();
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }
};

int main(int argc, char* argv[])
{
    GlobalStat gameStat;
    std::thread orcThread(OrcsUglyWorld(), &gameStat);
    std::thread dragonThread(DragonsDirtyWorld(), &gameStat);
    std::thread heroThread(HeroFromLudwigshafen(), &gameStat);
    orcThread.join();
    dragonThread.join();
    heroThread.join();
    return 0;
}
