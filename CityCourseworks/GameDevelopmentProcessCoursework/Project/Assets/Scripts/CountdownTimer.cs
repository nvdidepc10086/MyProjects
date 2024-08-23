using System;
using UnityEngine;
using TMPro;

public class CountdownTimer : MonoBehaviour
{

    [SerializeField] TextMeshProUGUI timerText;
    float remainingTime = 90;
    bool isCountdownStarted = false;
    bool isCountdownFinished = false;

    // Update is called once per frame
    void Update()
    {
        if (GameObject.FindObjectOfType<MissionGetPos>().isCollisionMissionGetPos && !isCountdownStarted)
        {
            isCountdownStarted = true;
        }

        
        if (GameObject.FindObjectOfType<MissionFinishPos>().isCollisionMissionFinishPos && !isCountdownFinished)
        {
            isCountdownFinished = true;
        }
        

        if (isCountdownStarted == true && isCountdownFinished == false)
        {
            if (remainingTime > 60)
            {
                remainingTime -= Time.deltaTime;
                timerText.color = Color.green;
            }
            else if (remainingTime <= 60 && remainingTime > 30)
            {
                remainingTime -= Time.deltaTime;
                timerText.color = Color.yellow;
            }
            else if (remainingTime <= 30 && remainingTime > 0)
            {
                remainingTime -= Time.deltaTime;
                timerText.color = Color.red;
            }
            else if (remainingTime < 0)
            {
                remainingTime = 0;

            }

            int minutes = Mathf.FloorToInt(remainingTime / 60);
            int seconds = Mathf.FloorToInt(remainingTime % 60);

            timerText.text = string.Format("Remaining: ") + string.Format("{0:00}:{1:00}", minutes, seconds);
        }
        else if (isCountdownStarted == true && isCountdownFinished == true)
        {
            timerText.text = string.Format("Waiting For Next Order");
            isCountdownStarted = false;
            isCountdownFinished = false;
            print(isCountdownStarted);
            print(isCountdownFinished);
        } 
    }
}
