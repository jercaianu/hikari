#include "hikari/client/game/objects/HeroClimbingMobilityState.hpp"
#include "hikari/client/game/objects/HeroAirbornMobilityState.hpp"
#include "hikari/client/game/objects/HeroIdleMobilityState.hpp"
#include "hikari/core/game/SpriteAnimator.hpp"
#include "hikari/core/game/map/Room.hpp"

namespace hikari {

    Hero::ClimbingMobilityState::ClimbingMobilityState(Hero & hero)
        : MobilityState(hero)
    {

    }

    Hero::ClimbingMobilityState::~ClimbingMobilityState() {

    }

    void Hero::ClimbingMobilityState::enter() {
        hero.isOnLadder = true;
        hero.isFalling = false;
        hero.isAirborn = false;
        hero.isFullyAccelerated = false;
        hero.isJumping = false;
        hero.isStanding = false;
        hero.isDecelerating = false;
        hero.isWalking = false;

        hero.setVelocityX(0.0f);
        hero.setPosition(static_cast<float>(hero.ladderPositionX), hero.getPosition().getY());
        hero.body.setGravitated(false);
        hero.body.setTreatLadderTopAsGround(false);
    }

    void Hero::ClimbingMobilityState::exit() {
        hero.isOnLadder = false;
        hero.body.setGravitated(true);
        hero.body.setTreatLadderTopAsGround(true);
        hero.getAnimationPlayer()->unpause();
    }

    Hero::MobilityState::StateChangeAction Hero::ClimbingMobilityState::update(const float & dt) {
        hero.body.setTreatLadderTopAsGround(false);

        if(hero.actionController) {
            auto const * controller = hero.actionController.get();

            hero.setVelocityY(0);
            hero.getAnimationPlayer()->pause();

            if(controller->shouldMoveUp()) {
                if(hero.isTouchingLadderTop) {
                    hero.changeAnimation("climbing-top");
                } else {
                    hero.changeAnimation("climbing");
                }
                hero.setVelocityY(-hero.climbVelocity.getY());
                hero.getAnimationPlayer()->unpause();

                // If we climb to the top of the ladder, we need to be put in the ground instead of move up too high.
                // TODO: clean this code up... it's messy!
                if(!hero.isTouchingLadder) {
                    hero.setVelocityY(0.0f);

                    auto positionPixels = hero.body.getPosition();
                    auto offsetPixels = hero.body.getBoundingBox().getOrigin();
                    auto gridSize = hero.getRoom()->getGridSize();
                    auto bottom = hero.body.getBoundingBox().getBottom();
                    int newBottom = static_cast<int>(std::ceil(bottom) / static_cast<float>(gridSize)) * gridSize; // Quantize the bottom pixels
                    int newY = newBottom;// + offsetPixels.getY();
                    hero.body.setPosition(positionPixels.getX(), static_cast<float>(newY));
                    hero.body.setOnGround(true);
                    // HIKARI_LOG(debug4) << "new y = " << newY;
                }
            } else if(controller->shouldMoveDown()) {
                if(hero.isTouchingLadderTop) {
                    hero.changeAnimation("climbing-top");
                } else {
                    hero.changeAnimation("climbing");
                }
                hero.setVelocityY(hero.climbVelocity.getY());
                hero.getAnimationPlayer()->unpause();
            } else if(controller->shouldJump()) {
                // If you're holding up or down the jump button is ignored
                // So that's why it's at the end of this if/else branch
                hero.isFalling = true;
                hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                return MobilityState::NEXT;
            }

            // Only change directions if you're shooting
            // If you're not shooting then you can't change directions left/right

            // Can jump (amd fall down) from ladders


            if(hero.body.isOnGround() && !hero.isTouchingLadderWithFeet) {
                hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                return MobilityState::NEXT;
            }

            // Can reach the top of a ladder and not be climbing anymore
            if(!hero.isTouchingLadder && !hero.isTouchingLadderWithFeet) {
                if(controller->shouldMoveDown()) {
                    hero.isFalling = true;
                    hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new AirbornMobilityState(hero)));
                    return MobilityState::NEXT;
                }

                hero.requestMobilityStateChange(std::unique_ptr<MobilityState>(new IdleMobilityState(hero)));
                return MobilityState::NEXT;
            }
        }

        return MobilityState::CONTINUE;
    }

} // hikari