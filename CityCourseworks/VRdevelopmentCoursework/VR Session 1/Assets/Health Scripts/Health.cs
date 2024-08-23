using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
[ExecuteInEditMode]
public class Health : MonoBehaviour
{
    [SerializeField]
	// Starting health value
    private float _health = 100.0f;
	
	// Message to explain current health status in the Unity Inspector for Debugging
    public string status = "Full Health";

	// Unity events that allow you to trigger actions through the inspector when certain criteria within the script are met. 
    public UnityEvent OnHeal;
    public UnityEvent OnDeath;
    public UnityEvent OnHurt;
    public UnityEvent OnCombat;

	// Public float that allows you to get this character / objects health from another script.
    public float GetHealth()
    {
        return _health;
    }
	// Public float that allows you to set this character / objects health to a set amount from another script or the inspector.
    public void SetHealth(float health)
    {
        _health = health;
    }

	// Public function that allows you to access increase this character / objects health by a certain amount from another script or the inspector.
    public void IncreaseHealth(float health)
    {
        if (_health < 100 - health)
        {
            _health = _health + health;
        } else
        {
            _health = 100;
        }
    }

	// Public bool indicating if the character / object is currently in combat. Updating the status message and triggering appropriate coroutines. 
    public bool InCombat(bool _combatStatus)
    {
        if(_combatStatus == true)
        {
         StartCoroutine("CheckCombatStatus", _health); // Starts coroutine to determine if character is in combat. 
            status = "In Combat";
            return _combatStatus;
        } else {
            StartCoroutine(RegenerateHealth(3.0f)); // Calls a coroutine to start regenerating the character / objects health by a set wait time. 
            return _combatStatus;
        }
      
    }

	// Public function to apply a set amount of projectile damage to the character / object - can be called by other objects - e.g. a bullet hitting them. 
    public void ProjectileDamage(float amount)
    {
        Damage("Bullet", amount);
    }

	// Public function to apply a set amount of melee damage to the character / object - can be called by other objects - e.g. a sword hitting them. 
	   public void MeleeDamage(float amount)
    {
        Damage("Melee", amount);
    }

	// Public function to apply a set amount of fall damage to the character / object. 
	   public void FallDamage(float amount)
    {
        Damage("Fall", amount);
    }


        public void Damage(string type, float amount)
    {
        OnHurt.Invoke();
        switch (type)
        {
            case "Melee":
                SetHealth(_health - amount);
				InCombat(true);
                break;
            case "Bullet":
                SetHealth(_health - amount);
                InCombat(true);
                break;
            case "Environment":
                SetHealth(_health - amount);
                break;
            case "Fall":
                SetHealth(_health - amount);
                break;
        }
    }

    void Die()
    {
        this.GetComponent<Animator>().SetBool("isDead", true);
       // this.GetComponent<CharacterController>().enabled = false;
    }

    // Update is called once per frame
    void Update()
    {
		// Checks player health every frame. Updating status if full health and runs OnDeath UnityEvents if health under 0.
      if(_health == 100)
        {
            status = "Full Health";
        }

      if(_health < 0)
        {
            status = "Dead";
			OnDeath.Invoke();
        }
    }
	
	// Coroutine to regenerate health by a set amount after a set number of seconds. 
    private IEnumerator RegenerateHealth(float waitTime)
    {
        while (_health < 100)
        {
            yield return new WaitForSeconds(waitTime);
            IncreaseHealth(2.0f); // Calls increate health function to update player health by set amount.
            status = "Healing"; // Updates status message to Healing
            OnHeal.Invoke(); // Runs OnHeal UnityEvents whilst the player is healing. 
        } 
          

    }

	// Checks players health against starting health after a set wait time to see if it has increased, to determine if they are still in combat or healing. 
    private IEnumerator CheckCombatStatus(float startingHealth)
    {
        yield return new WaitForSeconds(3.0f); // Time to wait between checks. 
        if (_health >= startingHealth)
        {
            InCombat(false);
            StopCoroutine("CheckCombatStatus");
        } else {
		    OnCombat.Invoke(); // If health hasn't increased invoke OnCombat UnityEvents.
		}

    }
}
