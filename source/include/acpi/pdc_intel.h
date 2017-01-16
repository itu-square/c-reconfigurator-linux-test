// /*AFLA*/ 
// /*AFLA*/ /* _PDC bit definition for Intel processors */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __PDC_INTEL_H__
// /*AFLA*/ #define __PDC_INTEL_H__
// /*AFLA*/ 
// /*AFLA*/ #define ACPI_PDC_P_FFH			(0x0001)
// /*AFLA*/ #define ACPI_PDC_C_C1_HALT		(0x0002)
// /*AFLA*/ #define ACPI_PDC_T_FFH			(0x0004)
// /*AFLA*/ #define ACPI_PDC_SMP_C1PT		(0x0008)
// /*AFLA*/ #define ACPI_PDC_SMP_C2C3		(0x0010)
// /*AFLA*/ #define ACPI_PDC_SMP_P_SWCOORD		(0x0020)
// /*AFLA*/ #define ACPI_PDC_SMP_C_SWCOORD		(0x0040)
// /*AFLA*/ #define ACPI_PDC_SMP_T_SWCOORD		(0x0080)
// /*AFLA*/ #define ACPI_PDC_C_C1_FFH		(0x0100)
// /*AFLA*/ #define ACPI_PDC_C_C2C3_FFH		(0x0200)
// /*AFLA*/ #define ACPI_PDC_SMP_P_HWCOORD		(0x0800)
// /*AFLA*/ 
// /*AFLA*/ #define ACPI_PDC_EST_CAPABILITY_SMP	(ACPI_PDC_SMP_C1PT | \
// /*AFLA*/ 					 ACPI_PDC_C_C1_HALT | \
// /*AFLA*/ 					 ACPI_PDC_P_FFH)
// /*AFLA*/ 
// /*AFLA*/ #define ACPI_PDC_EST_CAPABILITY_SWSMP	(ACPI_PDC_SMP_C1PT | \
// /*AFLA*/ 					 ACPI_PDC_C_C1_HALT | \
// /*AFLA*/ 					 ACPI_PDC_SMP_P_SWCOORD | \
// /*AFLA*/ 					 ACPI_PDC_SMP_P_HWCOORD | \
// /*AFLA*/ 					 ACPI_PDC_P_FFH)
// /*AFLA*/ 
// /*AFLA*/ #define ACPI_PDC_C_CAPABILITY_SMP	(ACPI_PDC_SMP_C2C3  | \
// /*AFLA*/ 					 ACPI_PDC_SMP_C1PT  | \
// /*AFLA*/ 					 ACPI_PDC_C_C1_HALT | \
// /*AFLA*/ 					 ACPI_PDC_C_C1_FFH  | \
// /*AFLA*/ 					 ACPI_PDC_C_C2C3_FFH)
// /*AFLA*/ 
// /*AFLA*/ #endif				/* __PDC_INTEL_H__ */
