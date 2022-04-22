#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CActionComponent.h"

#include "Widget/CWidget_Player.h"
#include "Widget/CWidget_Selecter.h"
#include "Blueprint/UserWidget.h"
#include "Actions/CGhost.h"

ACPlayer::ACPlayer()
{
	// Tick 함수 실행 여부
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Player/Curve_Basic.Curve_Basic'");

	// Scene Component
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	// Actor Component
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCOptionComponent>(this, &Option, "Option");
	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");

	// Component Setting
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/Mannequin/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> anim;
	CHelpers::GetClass<UAnimInstance>(&anim, "AnimBlueprint'/Game/Player/ABP_CPlyaer.ABP_CPlyaer_C'");
	GetMesh()->SetAnimInstanceClass(anim);

	// Widget Class Set
	CHelpers::GetClass<UCWidget_Player>(&WidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Player.WB_Player_C'");
	CHelpers::GetClass<UCWidget_Selecter>(&ItemWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Selecter.WB_Selecter_C'");

	// PoseableMesh Set
	CHelpers::GetClass<ACGhost>(&Ghost, "Blueprint'/Game/Actions/BP_CGhost.BP_CGhost_C'");

	// Block_AnimMontage Set
	CHelpers::GetAsset<UAnimMontage>(&Block_AnimMontage, "AnimMontage'/Game/Character/Animation/Montages/G2_Warrior/Frank_RPG_Warrior_Block_Montage.Frank_RPG_Warrior_Block_Montage'");

	SpringArm->SetRelativeLocationAndRotation(FVector(0, 0, 140), FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;

	Camera->SetRelativeLocation(FVector(0, 50, 0));

	bUseControllerRotationYaw = false;	
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerWidget = CreateWidget<UCWidget_Player, APlayerController>(GetController<APlayerController>(), WidgetClass);
	PlayerWidget->AddToViewport();

	ItemWidget = CreateWidget<UCWidget_Selecter, APlayerController>(GetController<APlayerController>(), ItemWidgetClass);
	ItemWidget->AddToViewport();
	ItemWidget->SetVisibility(ESlateVisibility::Hidden);

	PlayerWidget->OnPlayFadeScreen();
	
	TimelineFoat.BindUFunction(this, "OnTimeLine_WalkSpeed");
	Timeline.AddInterpFloat(Curve, TimelineFoat);
	Timeline.SetPlayRate(5.0f);	//배속

}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	// Evade
	if (State->IsEvadeMode())
	{
		if (Action->IsWarriorMode())
		{
			StopAnimMontage();
			PlayAnimMontage(Block_AnimMontage, 1, NAME_None);
		}

		PlayGhost();
		UKismetSystemLibrary::K2_SetTimer(this, "PlayGhost", 0.1f, false);
		//UKismetSystemLibrary::K2_SetTimer(this, "PlayGhost", 0.2f, false); 
		return 0.0f;
	}

	// Hit
	Status->SubHealth(Damage);

	PlayerWidget->OnPlayBloodScreen();

	if (Status->GetHealth() <= 0)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	return Status->GetHealth();
}

void ACPlayer::PlayGhost()
{
	FTransform tr = GetMesh()->GetComponentTransform();
	ACGhost* ghost = GetWorld()->SpawnActor<ACGhost>(Ghost, tr);
	//UGameplayStatics::FinishSpawningActor(ghost, tr);
	ghost->Init(GetMesh());
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("ZoomCtrl", this, &ACPlayer::OnZoomCtrl);

	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OffWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OnWalk);

	PlayerInputComponent->BindAction("Warrior", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarrior);
	PlayerInputComponent->BindAction("Berserker", EInputEvent::IE_Pressed, this, &ACPlayer::OnBerserker);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
	PlayerInputComponent->BindAction("Action_pp", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction_pp);

	PlayerInputComponent->BindAction("Space", EInputEvent::IE_Pressed, this, &ACPlayer::OnSpace);
	PlayerInputComponent->BindAction("Space", EInputEvent::IE_Released, this, &ACPlayer::OffSpace);
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove());
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnZoomCtrl(float InAxis)
{
	SpringArm->TargetArmLength += (InAxis * Option->GetZoomSpeed() * GetWorld()->GetDeltaSeconds());
	SpringArm->TargetArmLength = FMath::Clamp(
		SpringArm->TargetArmLength, 
		Option->GetMinZoomRange(), 
		Option->GetMaxZoomRange()
	);
}

void ACPlayer::OnWalk()
{
	Timeline.ReverseFromEnd();
}

void ACPlayer::OffWalk()
{
	Timeline.PlayFromStart();
}

void ACPlayer::OnTimeLine_WalkSpeed(float Value)
{
	GetCharacterMovement()->MaxWalkSpeed = Value;
}

void ACPlayer::OnUnarmed()
{
	CheckFalse(State->IsIdleMode());
	Action->SetUnarmedMode();
}

void ACPlayer::OnWarrior()
{
	CheckFalse(State->IsIdleMode());

	Action->SetWarriorMode();
}

void ACPlayer::OnBerserker()
{
	CheckFalse(State->IsIdleMode());

	Action->SetBerserkerMode();
}


void ACPlayer::OnDoAction()
{
	Action->DoAction();
	//UKismetSystemLibrary::K2_SetTimer(this, "PlayGhost", 1.0f, false);
	//PlayerWidget->OnPlayFadeScreen();
}

void ACPlayer::OnDoAction_pp()
{
	CheckFalse(State->IsEvadeMode());
	Action->DoCounter();
}

void ACPlayer::OnSpace()
{
	bSpace = true;
	Action->DoEvade();

}

void ACPlayer::OffSpace()
{
	bSpace = false;
}

FGenericTeamId ACPlayer::GetGenericTeamId() const 
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::OnItemWidget()
{
	APlayerController* controller = Cast<APlayerController>(GetController());
	int32 x, y;
	controller->GetViewportSize(x, y);
	controller->SetMouseLocation(x / 2, y / 2);
	controller->bShowMouseCursor = true;
	controller->SetInputMode(FInputModeGameAndUI());
	ItemWidget->SetVisibility(ESlateVisibility::Visible);
	ItemWidget->OnStartAnimation();
	
}

void ACPlayer::OffItemWidget()
{
	APlayerController* controller = Cast<APlayerController>(GetController());
	controller->bShowMouseCursor = false;
	controller->SetInputMode(FInputModeGameOnly());
	ItemWidget->SetVisibility(ESlateVisibility::Hidden);
}