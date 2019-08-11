using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Creation : MonoBehaviour
{
    public Inventory Iv;

    public Slot it1;
    public Slot it2;

    public int select_list_num;

    public Slot[] item_slot;

    public Button bb;
    public Text text;
    public Image img;

    // Start is called before the first frame update
    void Start()
    {
        Iv = GameObject.FindGameObjectWithTag("Inventory").GetComponent<Inventory>();

        it1 = Iv.gameObject.transform.GetChild(0).GetComponent<Slot>();
        it2 = Iv.gameObject.transform.GetChild(1).GetComponent<Slot>();

        //for (int i = 0; i < Iv.AllSlot.Count; i++)
        //{
        //    if (Iv.AllSlot[i].GetComponent<Slot>().ItemReturn().type == Item.TYPE.Box)
        //        item_slot[0] = Iv.AllSlot[i].GetComponent<Slot>();
        //    else if (Iv.AllSlot[i].GetComponent<Slot>().ItemReturn().type == Item.TYPE.Crystal)
        //        item_slot[1] = Iv.AllSlot[i].GetComponent<Slot>();
        //}

        select_list_num = 1;

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.M))
            Debug.Log("슬롯 개수" + Iv.AllSlot.Count);
    }

    public void List1Button()
    {
        select_list_num = 1;
        text = transform.Find("Button").gameObject.transform.Find("Text").gameObject.GetComponent<Text>();
        if (it1.ItemReturn().type == Item.TYPE.Box)
            text.text = it1.slot.Count.ToString() + "/1";
   
        else
            text.text = it2.slot.Count.ToString() + "/1";
    }

    public void List1Create()
    {
        if (it1.ItemReturn().type == Item.TYPE.Box && it1.slot.Count >= 1)
        {
            img.gameObject.SetActive(true);
            it1.ItemUse();
            text.text = it1.slot.Count.ToString() + "/1";
        }

        if (it2.ItemReturn().type == Item.TYPE.Box && it2.slot.Count >= 1)
        {
            img.gameObject.SetActive(true);
            it2.ItemUse();
            text.text = it2.slot.Count.ToString() + "/1";
        }
    }

}
