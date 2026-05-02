#pragma once


#include "AbilityCaller.h"
#include <set>
#include <concepts>

class Ability;

template<typename T>
concept GenericAbility = std::derived_from<T, Ability>;

class AbilityManager {

public:

    template<GenericAbility GenericAbility_t>
    static void GiveAbility(AbilityCaller* robot,GenericAbility_t* ability) {

        ability->AbilityOwner = robot;

        robot->accessible_abilities.emplace_back(ability);
    }

    static bool HasAbility(Ability* ability, AbilityCaller* Caller) {
        for (Ability* accessible_ability : Caller->accessible_abilities) {

            if (accessible_ability == ability) {
                return true;
            }

        }

        return false;
    }

    static bool HasTag(const std::string& tag, AbilityCaller* Caller) {

        for (std::string caller_tag : Caller->active_abilities_tags)
        {
            if (caller_tag == tag) {
                return true;
            }
        }

        return false;
    }

    static void TryActivateAbility_ByObject(Ability* ability, AbilityCaller* Caller) {

        if (HasAbility(ability, Caller)) {
            Caller->active_abilities_tags.push_back(ability->getAbilityTag());

            ability->EVENT_ActivateAbility();

            std::erase(Caller->active_abilities_tags, ability->getAbilityTag());
        }

    }

    static void TryActivateAbility_ByTag(const std::string& tag, AbilityCaller* Caller) {

        for (int i = 0; i < Caller->accessible_abilities.size(); i++)
        {
            if (Caller->accessible_abilities[i]->getAbilityTag() == tag) {

                Caller->active_abilities_tags.push_back(tag);

                Caller->accessible_abilities[i]->EVENT_ActivateAbility();

                std::erase(Caller->active_abilities_tags, tag);
            }
        }
    }

    template<GenericAbility AbilityType>
    static void TryActivateAbility_ByClass(AbilityCaller* Caller)
    {
        for (Ability* ability : Caller->accessible_abilities)
        {
            if (auto* derived = dynamic_cast<AbilityType*>(ability))
            {
                Caller->active_abilities_tags.push_back(derived->getAbilityTag());

                derived->EVENT_ActivateAbility();

                std::erase( Caller->active_abilities_tags, derived->getAbilityTag());

                return;
            }
        }
    }

};

template<typename T>
concept IsNumerical = std::integral<T> || std::floating_point<T>;

template<IsNumerical Ty>
class RobotAbilityAttribute {

    Ty m_Value;

    std::string m_Name;

public:

    Ty operator=(const Ty& other) {
        this->m_Value = other;

        return { other };
    }

    constexpr Ty* get() noexcept {
        return &m_Value;
    }

    RobotAbilityAttribute(const Ty& value, const std::string& name) : m_Value(value), m_Name(name) {}
};

enum class EAbilityEffectType : uint16_t
{
    SUBTRACTER,
    ADDER,
    MULTIPLIER,
    DIVIDER
};

template<EAbilityEffectType EffectTy, typename AttributeType>
class AbilityEffect
{
private:

    bool m_bAffectOverTime = false;

    float m_Rate = 1.0f;

protected:

    std::string Name;

    RobotAbilityAttribute<AttributeType>* AffectedAttribute;

    float Magnitude;

public:

    AbilityEffect(const std::string& Name, RobotAbilityAttribute<AttributeType>* AffectedAttribute, float Magnitude) : Name(Name), AffectedAttribute(AffectedAttribute), Magnitude(Magnitude) {
    }

public:

    constexpr RobotAbilityAttribute<AttributeType>* getAffectedAttribute() const {
        return AffectedAttribute;
    }

    constexpr float* getEffectMagnitude() noexcept {
        return &Magnitude;
    }

    void ExecuteEffect(){

        switch (EffectTy)
        {
        case EAbilityEffectType::SUBTRACTER:

            *AffectedAttribute->get() = *AffectedAttribute->get() - Magnitude;

            break;
        case EAbilityEffectType::ADDER:

            *AffectedAttribute->get() = *AffectedAttribute->get() + Magnitude;

            break;
        case EAbilityEffectType::MULTIPLIER:

            *AffectedAttribute->get() = *AffectedAttribute->get() * Magnitude;

            break;
        case EAbilityEffectType::DIVIDER:

            *AffectedAttribute->get() = *AffectedAttribute->get() / Magnitude;

            break;
        default:
            break;
        }

        
    }

};


class AbilityEffectsManager
{
public:

    template<EAbilityEffectType FX_Type, IsNumerical AttribTy>
    static void CommitEffect()
    {
        AbilityEffect<FX_Type, AttribTy> Effect;

        Effect.ExecuteEffect();
    }

    template<EAbilityEffectType FX_Type, IsNumerical AttribTy>
    static void CommitEffect(AbilityEffect<FX_Type, AttribTy>* Effect) {

        Effect->ExecuteEffect();

    }


};
