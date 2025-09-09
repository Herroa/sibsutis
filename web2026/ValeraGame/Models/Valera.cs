namespace ValeraGame.Models;

public class Valera
{
  public int Health { get; private set; } = 100;
  public int Alcohol { get; private set; } = 0;
  public int Happiness { get; private set; } = 0;
  public int Fatigue { get; private set; } = 0;
  public int Money { get; private set; } = 0;

  public Valera() { }

  private void ClampValues()
  {
    Health = Math.Clamp(Health, 0, 100);
    Alcohol = Math.Clamp(Alcohol, 0, 100);
    Happiness = Math.Clamp(Happiness, -10, 10);
    Fatigue = Math.Clamp(Fatigue, 0, 100);
  }

  public bool CanWork => Alcohol < 50 && Fatigue < 10;

  public void Work()
  {
    if (!CanWork)
      throw new InvalidOperationException("Can't work, too fatigued or drunk");
    Happiness -= 5;
    Alcohol -= 30;
    Money += 100;
    Fatigue += 70;
    ClampValues();
  }

  public void Walk()
  {
    Happiness += 1;
    Alcohol -= 10;
    Fatigue += 10;
    ClampValues();
  }

  public void Series()
  {
    Happiness -= 1;
    Alcohol += 30;
    Fatigue += 10;
    Health -= 5;
    Money -= 20;
    ClampValues();
  }

  public void Pub()
  {
    Happiness += 1;
    Alcohol += 60;
    Fatigue += 40;
    Health -= 10;
    Money -= 100;
    ClampValues();
  }

  public void Drink()
  {
    Happiness += 5;
    Health -= 80;
    Alcohol += 90;
    Fatigue += 80;
    Money -= 150;
    ClampValues();
  }

  public void Sing()
  {
    Happiness += 1;
    Alcohol += 10;
    Fatigue += 20;

    if (Alcohol >= 40 && Alcohol <= 70)
      Money += 50;
    else
      Money += 10;

    ClampValues();
  }

  public void Sleep()
  {
    if (Alcohol < 30)
      Health += 90;

    if (Alcohol > 70)
      Happiness -= 3;

    Alcohol -= 50;
    Fatigue -= 70;
    ClampValues();
  }
}