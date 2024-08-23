using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PhoneMenu : MonoBehaviour
{
    public static bool SocialMediaIsOpen = false;
    public GameObject SocialMediaMenuUI;

    /*
    public void LikeButton()
    {
        Debug.Log("give like to this blog");
    }

    public void DisLikeButton()
    {
        Debug.Log("give dislike to this blog");
    }
    */

    /*
    public void EnterSocialMediaOrBackToPhoneScreen()
    {
        if (!SocialMediaIsOpen)
        {
            OpenSocialMediaMenu();
        }
        else
        {
            CloseSocialMediaMenu();
        }
    }
    */

    public void ExitToGameWorld()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex - 1);
    }

    public void CollectOrderButton()
    {
        print("Order Collect");
        //Debug.Log("Car Shopping Menu is opened");
    }

    public void FinishOrderButton()
    {
        print("Order Finished");
    }

    /*
    public void OpenClothShoppingMenu()
    {
        Debug.Log("Cloth Shopping Menu is opened");
    }

    public void OpenHouseShoppingMenu()
    {
        Debug.Log("House Shopping Menu is opened");
    }
    */

    /*
    void OpenSocialMediaMenu()
    {
        SocialMediaMenuUI.SetActive(true);
        Time.timeScale = 1f;
        SocialMediaIsOpen = true;

    }

    void CloseSocialMediaMenu()
    {
        SocialMediaMenuUI.SetActive(false);
        Time.timeScale = 0f;
        SocialMediaIsOpen = false;
    }
    */
}
