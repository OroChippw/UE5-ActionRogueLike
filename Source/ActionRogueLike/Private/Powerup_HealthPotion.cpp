// Fill out your copyright notice in the Description page of Project Settings.

#include "Powerup_HealthPotion.h"

#include "AttributeComponent.h"

// #define LOCTEXT_NAMESPACE 是 Unreal Engine 中用于定义文本本地化命名空间的预处理宏
// 可以帮助在翻译不同语言的时候，确保每个字符串都有唯一的标识符，以便正确地进行翻译和本地化。
#define LOCTEXT_NAMESPACE "InteractableActors"

APowerup_HealthPotion::APowerup_HealthPotion()
{
    CreditCost = 50;
}

void APowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
    if (!ensure(InstigatorPawn))
    {
        return ;
    }

    UAttributeComponent* AttributeComp = UAttributeComponent::GetAttribute(InstigatorPawn);
    if (ensure(AttributeComp) && !AttributeComp->isFullHealth())
    {
        // TODO
    }
}

FText APowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
    UAttributeComponent* AttributeComp = UAttributeComponent::GetAttribute(InstigatorPawn);
    if (AttributeComp && AttributeComp->isFullHealth())
    {   
        // 直接使用LOCTEXT适用于静态文本，无需进行格式化
        return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
    }

    // 这里使用FText::Format是因为存在占位符 {0} 表示将被替换为 CreditCost 变量的值。这种方式适用于动态文本，需要根据参数值来构建最终的文本
    return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost); 
}

#undef LOCTEXT_NAMESPACE