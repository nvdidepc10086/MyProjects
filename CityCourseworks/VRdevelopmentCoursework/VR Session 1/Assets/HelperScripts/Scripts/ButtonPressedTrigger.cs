using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEditor;
using UnityEngine.InputSystem;
using UnityEngine.InputSystem.Utilities;
using static UnityEngine.InputSystem.InputAction;

[ExecuteInEditMode]
public class ButtonPressedTrigger : MonoBehaviour
{
    Matrix4x4 thisTriggerMatrix;
    public InputAction UseButton;
    public string thisTriggerName;
    public string targetTag;
    public bool canToggle = true;
    private bool activeState = false;
    private bool buttonDown = false;
    public Color triggerColour = Color.cyan;
    Color triggerFillColour;
    public UnityEvent OnUsed; 
    public UnityEvent OnTriggerEnterEvent;
    public UnityEvent OnTriggerStayEvent;
    public UnityEvent OnTriggerExitEvent;
  


    void OnEnable()
    {
      
        this.gameObject.layer = 2;
        this.GetComponent<Collider>().isTrigger = true;
        if (this.GetComponent<MeshRenderer>() == true)
        {
            DestroyImmediate(this.GetComponent<MeshRenderer>());
        }
       

    }

    void OnAwake()
    {
        UseButton.Enable();
    }

    void OnValidate()
    {
        triggerFillColour = new Color(triggerColour.r, triggerColour.g, triggerColour.b, 0.25f);
        gameObject.name = thisTriggerName + " (Button Trigger)";
    }

#if UNITY_EDITOR
    void OnDrawGizmos()
    {
        thisTriggerMatrix = this.transform.localToWorldMatrix;
        Gizmos.matrix = thisTriggerMatrix;
        Gizmos.color = triggerColour;
        Gizmos.DrawWireCube(GetComponent<BoxCollider>().center, GetComponent<BoxCollider>().size); //DrawCube does filled Cube
        Gizmos.color = triggerFillColour;
        Gizmos.DrawCube(GetComponent<BoxCollider>().center, GetComponent<BoxCollider>().size); //DrawCube does filled Cube
        GUIStyle handleStyle = new GUIStyle();
        handleStyle.alignment = TextAnchor.MiddleCenter;
        handleStyle.fontStyle = FontStyle.BoldAndItalic;
        handleStyle.normal.textColor = Color.white;
        Handles.Label(transform.position, gameObject.name, handleStyle);
    }
#endif

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == targetTag)
        {
            OnTriggerEnterEvent.Invoke();
        }
    }

    private void OnTriggerStay(Collider other)
    {
        if (other.gameObject.tag == targetTag)
        {
            OnTriggerStayEvent.Invoke();

            if (UseButton.IsPressed() && buttonDown == false)
            {
                activeState = true;
                buttonDown = true;
            }

            if (!UseButton.IsPressed())
            {
                buttonDown = false;
            }
        }

    }

    public void Use(CallbackContext fireCTX)
    {
        if (fireCTX.performed)
        {
            activeState = true;
        }
    }


            private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.tag == targetTag)
        {
            OnTriggerExitEvent.Invoke();
        }

    }

    private void Update()
    {
        if (activeState == true)
        {
            OnUsed.Invoke();
            this.enabled = false;
        } 
    }


}
