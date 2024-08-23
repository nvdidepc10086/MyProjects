using System;
using UnityEngine;
using TMPro;

public class Gametime : MonoBehaviour
{

    [SerializeField] TextMeshProUGUI timerText;
    float elapsedTime;

    // Update is called once per frame
    void Update()
    {
        elapsedTime += Time.deltaTime;
        int minutes = Mathf.FloorToInt(elapsedTime / 60);
        int seconds = Mathf.FloorToInt(elapsedTime % 60);

        timerText.text = string.Format("GameTime: ") + string.Format("{0:00}:{1:00}", minutes, seconds);   

    }
}
