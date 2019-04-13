#ifndef _VM_TYPE_DEF_H__
#define  _VM_TYPE_DEF_H__
/*�������ͼ���������*/
typedef unsigned char	byte; 
typedef unsigned char* pbyte;
typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned int	uInt;

/*
	����ҳ���� = �����ڴ��С/ҳ���С
		256		= 256*64 / 64    
	����ҳ��256 ������8λ����ʾ
*/
#define  PM_PAGE_SIZE	256	//
#define  PM_SIZE	256*64  // �����ڴ��С4k(�ֽ���)
#define  PAGE_SIZE	64	// ҳ���С64�ֽ�


#define  VPN_LEN		10			//����ҳ��
#define	 PPN_LEN		8			//����ҳ��
#define  VPO_LEN		6			//����ҳƫ��
#define  PPO_LEN		6			//����ҳƫ�� ==����ҳƫ��
#define	VALID_LEN		1			//��Чλ
/*
	�����ַλ�� =  VPN(����ҳ�� 10λ)  + VPO(����ҳƫ�� 6λ)
	�����ַλ�� = PPN(����ҳ�� 8λ) + PPO(����ҳƫ�� 6λ)

	����ռ��С =  2^16 = 64KB 
*/
#define VM_ADDR_LEN  16  //�����ַλ��
#define  PM_ADDR_LEN 14  //�����ַλ��


/*
	���ٻ���ṹ˵����
		����Ѱַ ��ֱ��ӳ�䡢 �д�С4�ֽڣ��ܹ�16��
	������ CT(6λ) 
	����������CI(4λ)
	����ƫ�� CO(2λ)
	���λ
	���ֽڿ� (4�ֽڣ���ƫ����������)
*/
#define  CACHE_GROUP_SIZE		16  // 16�� 
#define ITEM_INDEX_LEN			4	//������
#define ITEM_TAG_LEN			8	//���

#define ITEM_OFFSET_LEN			2	//ƫ��λ��
#define ROW_BLOCK_SIZE			4	//�д�С
/*
	ҳ��˵����
		PTE(ҳ����Ŀ) = VPN(����ҳ��10)+PPN(����ҳ��6)+��Чλ(2)
		ҳ���С =  PTE��С* (�����ַ�ռ��С(2^16B)/ҳ���С(64B))	
*/
#define	 PAGE_ITEM_SIZE		1024	//2^10 PTE��Ŀ��
#define  PAGE_STATUS_LEN	2		//����ҳ״̬λ

/*
Vmini ���̴洢��ӳ��
����ҳ�� 1024ҳ
0~31		δ��	32
32~255		������ 224
256~511		���� �������� 256
512~767		ջ�� �������� 256
768~1023	�ں�����洢�� 256	
*/

//����洢����256ҳ
#define  VM_LOAD_BASE_PAGE		32		//�����ڴ���ʼ����ҳ
#define VM_LOAD_TAIL_PAGE		255		//������ֹҳ
#define	 PM_LOAD_BASE			64		//�����ڴ��п�ʼ���ص�ҳ������


/* TLB����󱸻�����
���� 16����Ŀ ��·������
TBL�ṹ��TLBT ��TLBI ����Чλ
TBLT+TLBI = VPN ����ҳ��λ��
*/
#define  TLBT_LEN   6			// ����λ��
#define  TLBI_LEN    2			//������λ�� 
#define  TLB_ITEM_SIZE 16  //TLB��Ŀ��

#endif
