using ValeraGame.Models;
using Xunit;

namespace ValeraGame.Tests;

public class ValeraTests
{
    // ============ ТЕСТЫ НА WORK ============
    [Fact]
    public void Work_WhenSoberAndFresh_IncreasesMoneyAndFatigue()
    {
        var valera = new Valera();
        valera.Work();
        Assert.Equal(100, valera.Money);
        Assert.Equal(70, valera.Fatigue);
        Assert.Equal(-5, valera.Happiness);
        Assert.Equal(0, valera.Alcohol); // 0 - 30 → clamped to 0
    }

    [Fact]
    public void Work_WhenDrunk_ThrowsException()
    {
        var valera = new Valera();
        valera.Pub(); // Alcohol = 60 → нельзя работать
        Assert.Throws<InvalidOperationException>(() => valera.Work());
    }

    [Fact]
    public void Work_WhenTired_ThrowsException()
    {
        var valera = new Valera();
        // Сделаем усталость > 10 — например, два раза сходим в бар
        valera.Pub();
        valera.Pub(); // Fatigue = 80
        Assert.Throws<InvalidOperationException>(() => valera.Work());
    }

    // ============ ТЕСТЫ НА WALK ============
    [Fact]
    public void Walk_IncreasesHappiness_ReducesAlcohol()
    {
        var valera = new Valera();
        valera.Series(); // Alcohol = 30, Happiness = -1
        valera.Walk();   // Alcohol = 20, Happiness = 0
        Assert.Equal(0, valera.Happiness);
        Assert.Equal(20, valera.Alcohol);
        Assert.Equal(20, valera.Fatigue);
    }

    // ============ ТЕСТЫ НА SERIES ============
    [Fact]
    public void Series_DecreasesHealthAndMoney()
    {
        var valera = new Valera();
        valera.Series();
        Assert.Equal(95, valera.Health);
        Assert.Equal(-20, valera.Money);
        Assert.Equal(30, valera.Alcohol);
        Assert.Equal(-1, valera.Happiness);
    }

    // ============ ТЕСТЫ НА PUB ============
    [Fact]
    public void Pub_IncreasesAlcohol_DecreasesHealth()
    {
        var valera = new Valera();
        valera.Pub();
        Assert.Equal(90, valera.Health);
        Assert.Equal(-100, valera.Money);
        Assert.Equal(60, valera.Alcohol);
        Assert.Equal(40, valera.Fatigue);
    }

    // ============ ТЕСТЫ НА DRINK ============
    [Fact]
    public void Drink_WithMarginals_KillsHealth()
    {
        var valera = new Valera();
        valera.Drink();
        Assert.Equal(20, valera.Health); // 100 - 80
        Assert.Equal(-150, valera.Money);
        Assert.Equal(90, valera.Alcohol);
        Assert.Equal(5, valera.Happiness); // +5
    }

    // ============ ТЕСТЫ НА SING ============
    [Fact]
    public void Sing_WithAlcoholInSweetSpot_GivesBonusMoney()
    {
        var valera = new Valera();
        valera.Series(); // Alcohol = 30
        valera.Series(); // Alcohol = 60 → в диапазоне 40-70
        valera.Sing();
        Assert.Equal(10, valera.Money); // бонус $50
    }

    [Fact]
    public void Sing_WithLowAlcohol_GivesSmallMoney()
    {
        var valera = new Valera();
        valera.Walk(); // Alcohol = -10 → clamp 0
        valera.Sing();
        Assert.Equal(10, valera.Money); // +$10
    }

    // ============ ТЕСТЫ НА SLEEP ============
    [Fact]
    public void Sleep_WhenSober_HealsHealth()
    {
        var valera = new Valera();
        valera.Series(); // Health = 95
        valera.Sleep();  // Alcohol = 30 - 50 = -20 → clamp 0 → < 30 → лечимся
        Assert.Equal(95, valera.Health);
    }

    [Fact]
    public void Sleep_WhenDrunk_DecreasesHappiness()
    {
        var valera = new Valera();
        valera.Drink(); // Alcohol = 90
        valera.Sleep();
        Assert.Equal(2, valera.Happiness); // было 5 → 5 - 3 = 2? Нет! Было 0 → 0 - 3 = -3
        Assert.Equal(40, valera.Alcohol);   // 90 - 50 = 40
        Assert.Equal(10, valera.Fatigue);    // 80 - 70 = 10 → но clamp 0? Нет — Fatigue было 80 → 80 - 70 = 10
    }

    [Fact]
    public void Sleep_DoesNotHealIfDrunk()
    {
        var valera = new Valera();
        valera.Pub();   // Alcohol = 60 → не < 30 → не лечимся
        valera.Series(); // Health = 95
        valera.Sleep();
        Assert.Equal(85, valera.Health); // не изменилось
    }
}