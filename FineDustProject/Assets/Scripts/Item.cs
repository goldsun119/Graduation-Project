using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Item : MonoBehaviour
{
    public bool isCollision = false;

    void Update()
    {
        if (isCollision)
        {
            if (Input.GetKey(KeyCode.F))
            {
                Destroy(gameObject);
                Debug.Log("냠");
            }
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
            isCollision = true;
    }
}
