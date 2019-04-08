using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Monster : MonoBehaviour
{
    public int HP;
    public bool isCollision = false;

    void Start()
    {
        HP = 100;
    }

    // Update is called once per frame
    void Update()
    {
        if(isCollision)
        {
            if (Input.GetMouseButtonUp(0)) //0:좌 1:중 2:우
            {
                HP -= 10;
                Debug.Log("피 "+HP+"만큼 남아따");
            }
        }

        if (HP == 0)
        {
            Destroy(gameObject);
            Debug.Log("나주거");
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.CompareTag("Player"))
        {
            isCollision = true;
        }
    }
}
