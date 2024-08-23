using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerController : MonoBehaviour
{
    [SerializeField] private float moveSpeed = 1f;
    [SerializeField] private float lookSensitivity = 5f;
    [SerializeField] private float jumpheight = 10;
    [SerializeField] private float gravity = 9.8f;
    private Vector2 moveVector;
    private Vector2 lookVector;
    private Vector3 rotation;
    private float verticalVelocity;
    private CharacterController characterController;
    private Animator animator;

    // Start is called before the first frame update
    void Start()
    {
        characterController = GetComponent<CharacterController>();
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        Move();
        Rotate();
    }

    public void OnMove(InputAction.CallbackContext context)
    {
        moveVector = context.ReadValue<Vector2>();
        if(moveVector.magnitude > 0)
        {
            animator.SetBool("isWalking", true);
        }
        else
        {
            animator.SetBool("isWalking", false);
        }
    }

    private void Move()
    {
        verticalVelocity += -gravity * Time.deltaTime;
        if(characterController.isGrounded && verticalVelocity < 0)
        {
            verticalVelocity = -0.1f * gravity * Time.deltaTime;
        }
        Vector3 move = transform.right * moveVector.x + transform.forward * moveVector.y + transform.up * verticalVelocity;
        characterController.Move(move * moveSpeed * Time.deltaTime);
    }

    public void OnLook(InputAction.CallbackContext context)
    {
        lookVector = context.ReadValue<Vector2>();
    }

    private void Rotate()
    {
        rotation.y += lookVector.x * lookSensitivity * Time.deltaTime;
        transform.localEulerAngles = rotation;
    }

    public void OnJump(InputAction.CallbackContext context)
    {
        if(characterController.isGrounded && context.performed) 
        {
            animator.Play("Standing Jump");
            //Jump();
        }
    }

    private void Jump()
    {
        verticalVelocity = Mathf.Sqrt(jumpheight * gravity);
    }

}
