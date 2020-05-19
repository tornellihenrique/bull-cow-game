// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Fala doido"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];

    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Fica shiu, mas a palavra eh: %s"), *HiddenWord);

    PrintLine(TEXT("Adivinha ae a palavra de %i letras"), HiddenWord.Len());
    PrintLine(TEXT("Tu tem %i vidas"), Lives);
    PrintLine(TEXT("Digite seu palpite \ne aperta enter pra da sequencia..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nAperta enter pra joga dnv..."));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
    if (Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("Ganhou, boa"));
        EndGame();

        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("Tem letra repetida doido, tenta dnv"));

        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("A palavra secreta tem %i letra doidao"), HiddenWord.Len());
        PrintLine(TEXT("Tenta dnv, ce ta com %i de vida"), Lives);

        return;
    }

    --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("As vida cabo, parabens... nss"));
        PrintLine(TEXT("A palavra secreta era: %s"), *HiddenWord);
        EndGame();

        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("Ce tem %i boi e %i vaca"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Errou nub"));
    PrintLine(TEXT("Tenta dnv, ce ta com %i de vida"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString &Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}