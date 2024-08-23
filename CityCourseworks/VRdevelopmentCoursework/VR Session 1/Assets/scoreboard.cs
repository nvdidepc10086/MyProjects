using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SocialPlatforms.Impl;
using UnityEngine.UI;

public class scoreboard : MonoBehaviour
{

    public static int Baskets = 0;
    // Start is called before the first frame update

    public Text myText;

    // Use this for initialization
    void Start()
    {

        myText.text = "";

    }


    // Update is called once per frame
    void Update()
    {

        myText.text = "" + Baskets;

    }
}
