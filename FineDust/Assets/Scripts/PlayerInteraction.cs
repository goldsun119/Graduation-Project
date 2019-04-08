using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerInteraction : MonoBehaviour
{
    public bool isItem = false;
    public bool isMon = false;

    void Update()
    {
        Item();
        Monster();
    }

    void Item()
    {
        if (isItem)
        {
            Debug.Log("아이템");
            isItem = false;
        }
    }

    void Monster()
    {
        if (isMon)
        {
            Debug.Log("몬스터");
            isMon = false;
        }
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Item"))
        {
            isItem = true;
        }

        if (collision.gameObject.CompareTag("Monster"))
        {
            isMon = true;
        }
    }
}
