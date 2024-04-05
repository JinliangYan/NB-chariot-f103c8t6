#include "sys.h"

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI
void
WFI_SET(void) {
    __ASM volatile("wfi");
}

//�ر������ж�
void
INTX_DISABLE(void) {
    __ASM volatile("cpsid i");
}

//���������ж�
void
INTX_ENABLE(void) {
    __ASM volatile("cpsie i");
}

// ����ջ����ַ
// addr:ջ����ַ
void
MSR_MSP(u32 addr) {
    __ASM volatile("MSR MSP, %0\n\t" // set Main Stack value
                   "BX LR\n\t"
                   :
                   : "r"(addr));
}
