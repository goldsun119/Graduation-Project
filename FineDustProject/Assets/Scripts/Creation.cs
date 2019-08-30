using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Creation : MonoBehaviour
{
    private Inventory Iv;

    private Slot it0;
    private Slot it1;

    public int select_list_num;

    private Slot[] item_slot;

    public Button bb;
    public Image img;
    private bool b1;

    public Button bb2;
    public Image img2;
    private bool b2;

    private Text text;

    public Button ending;


    // Start is called before the first frame update
    void Start()
    {
        Iv = GameObject.FindGameObjectWithTag("Inventory").GetComponent<Inventory>();
        
        it0 = Iv.gameObject.transform.GetChild(0).GetComponent<Slot>();
        it1 = Iv.gameObject.transform.GetChild(1).GetComponent<Slot>();

        //for (int i = 0; i < Iv.AllSlot.Count; i++)
        //{
        //    if (Iv.AllSlot[i].GetComponent<Slot>().ItemReturn().type == Item.TYPE.Box)
        //        item_slot[0] = Iv.AllSlot[i].GetComponent<Slot>();
        //    else if (Iv.AllSlot[i].GetComponent<Slot>().ItemReturn().type == Item.TYPE.Crystal)
        //        item_slot[1] = Iv.AllSlot[i].GetComponent<Slot>();
        //}

        select_list_num = 0;

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
        if (it0.ItemReturn().type == Item.TYPE.Box)
            text.text = it0.slot.Count.ToString() + "/1";
   
        else
            text.text = it1.slot.Count.ToString() + "/1";
    }

    public void List2Button()
    {
        select_list_num = 2;
        text = transform.Find("Button").gameObject.transform.Find("Text").gameObject.GetComponent<Text>();
        if (it1.ItemReturn().type == Item.TYPE.Box)
            text.text = it1.slot.Count.ToString() + "/1";

        else
            text.text = it0.slot.Count.ToString() + "/1";
    }


    public void List1Create()
    {
        // 걍 노가다로 때려박기
        if (select_list_num == 1)
        {
            //img.gameObject.SetActive(true);
            //bb.gameObject.SetActive(false);
            //b1 = true;
            // 요구하는 아이템 리스트 if문으로 다 걸으세요.
            if (it0.ItemReturn().type == Item.TYPE.Box && it0.slot.Count >= 1)
            {
                img.gameObject.SetActive(true);
                it0.ItemUse();
                text.text = it0.slot.Count.ToString() + "/1";
                bb.gameObject.SetActive(false);
                b1 = true;

                Game.Network.NetWork.SendCompleteMaking(1);
            }

            if (it1.ItemReturn().type == Item.TYPE.Box && it1.slot.Count >= 1)
            {
                img.gameObject.SetActive(true);
                it1.ItemUse();
                text.text = it1.slot.Count.ToString() + "/1";
                bb.gameObject.SetActive(false);
                b1 = true;
                Game.Network.NetWork.SendCompleteMaking(1);
            }


        }

        else if (select_list_num == 2)
        {
            //img2.gameObject.SetActive(true);
            //bb2.gameObject.SetActive(false);
            //b2 = true;
            if (it0.ItemReturn().type == Item.TYPE.Box && it0.slot.Count >= 1)
            {
                img2.gameObject.SetActive(true);
                it0.ItemUse();
                text.text = it0.slot.Count.ToString() + "/1";
                bb2.gameObject.SetActive(false);
                b2 = true;
                Game.Network.NetWork.SendCompleteMaking(2);
            }

            if (it1.ItemReturn().type == Item.TYPE.Box && it1.slot.Count >= 1)
            {
                img2.gameObject.SetActive(true);
                it1.ItemUse();
                text.text = it1.slot.Count.ToString() + "/1";
                bb2.gameObject.SetActive(false);
                b2 = true;
                Game.Network.NetWork.SendCompleteMaking(2);
            }

        }

        if (b1 && b2)
        {
            ending.gameObject.SetActive(true);
            
        }
    }

    public void Ending()
    {
        SceneManager.LoadScene(3);
    }
}
