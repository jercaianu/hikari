#include "hikari/client/scripting/GameProgressScriptProxy.hpp"
#include "hikari/client/game/GameProgress.hpp"

namespace hikari {

    std::weak_ptr<GameProgress> GameProgressScriptProxy::gameProgress = std::weak_ptr<GameProgress>();

    GameProgressScriptProxy::GameProgressScriptProxy() {
        // Private to prevent construction
    }

    GameProgressScriptProxy::GameProgressScriptProxy(const GameProgressScriptProxy& other) {
        // Private to prevent copy construction
    }

    std::weak_ptr<GameProgress> GameProgressScriptProxy::getWrappedService() {
        return gameProgress;
    }

    void GameProgressScriptProxy::setWrappedService(const std::weak_ptr<GameProgress> & gameProgress) {
        GameProgressScriptProxy::gameProgress = gameProgress;
    }

    int GameProgressScriptProxy::getLives() {
        if(auto progress = gameProgress.lock()) {
            return progress->getLives();
        }

        return 0;
    }

    int GameProgressScriptProxy::getETanks() {
        if(auto progress = gameProgress.lock()) {
            return progress->getETanks();
        }

        return 0;
    }

    int GameProgressScriptProxy::getMTanks() {
        if(auto progress = gameProgress.lock()) {
            return progress->getMTanks();
        }

        return 0;
    }

    void GameProgressScriptProxy::setLives(int value) {
        if(auto progress = gameProgress.lock()) {
            progress->setLives(value);
        }
    }

    void GameProgressScriptProxy::setETanks(int value) {
        if(auto progress = gameProgress.lock()) {
            progress->setETanks(value);
        }
    }

    void GameProgressScriptProxy::setMTanks(int value) {
        if(auto progress = gameProgress.lock()) {
            progress->setMTanks(value);
        }
    }

    void GameProgressScriptProxy::enableWeaponSlot(int slotIndex) {
        if(auto progress = gameProgress.lock()) {
            progress->enableWeapon(slotIndex, true);
        }
    }

    void GameProgressScriptProxy::disableWeaponSlot(int slotIndex) {
        if(auto progress = gameProgress.lock()) {
            progress->enableWeapon(slotIndex, false);
        }
    }

} // hikari
