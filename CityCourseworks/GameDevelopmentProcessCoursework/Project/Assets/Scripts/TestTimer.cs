using System;
using UnityEngine;
using TMPro;

public class TestTimer : MonoBehaviour
{

    [SerializeField] TextMeshProUGUI testTimerText;
    float countdowntimer = 90;
    bool countdownStarted = false;

    // Update is called once per frame
    void Update()
    {
        //print(countdowntimer);

        if (countdowntimer > 60)
        {
            countdowntimer -= Time.deltaTime;
            testTimerText.color = Color.green;
        }
        else if (countdowntimer <= 60 && countdowntimer > 30)
        {
            countdowntimer -= Time.deltaTime;
            testTimerText.color = Color.yellow;
        }
        else if (countdowntimer <= 30 && countdowntimer > 0)
        {
            countdowntimer -= Time.deltaTime;
            testTimerText.color = Color.red;
        }
        else if (countdowntimer < 0)
        {
            countdowntimer = 0;

        }

        int minutes = Mathf.FloorToInt(countdowntimer / 60);
        int seconds = Mathf.FloorToInt(countdowntimer % 60);

        testTimerText.text = string.Format("testTime: ") + string.Format("{0:00}:{1:00}", minutes, seconds);

    }
}
