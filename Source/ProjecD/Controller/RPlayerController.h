// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UUserWidget;

UCLASS()
class PROJECD_API ARPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ARPlayerController();

	// 캐릭터 입력 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<TObjectPtr<UInputAction>> SkillInputActions;


	// ===== HUD 세팅 =====
	// 로비에서 사용할 HUD (WBP_LobbyHUD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbyHUDClass;

	// 던전/인게임에서 사용할 HUD (WBP_InGameHUD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> InGameHUDClass;

	// 현재 화면에 떠 있는 HUD
	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentHUD;


	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "InputMode")
	void EnterGameControlMode();
	
	UFUNCTION(BlueprintCallable, Category = "InputMode")
	void EnterUIMode(class UUserWidget* FocusWidget);

private:
	// 맵 이름(Title / Lobby / Dungeon)에 따라 HUD 생성
	void SetupHUDForCurrentLevel();
};
