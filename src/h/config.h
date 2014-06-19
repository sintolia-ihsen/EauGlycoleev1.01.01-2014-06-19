
void config_Write(char *pBuffer, long Base, int Nbr, long IdConfig);
void config_Read(char *pBuffer, long Base, int Nbr, long IdConfig);

char *config_String(long Base, long IdConfig);
char config_Char(long Base, long IdConfig);
short config_Short(long Base, long IdConfig);
long config_Long(long Base, long IdConfig);
double config_Double(long Base, long IdConfig);


#define configGSM_Write(pBuffer, Base, Nbr) config_Write(pBuffer, Base, Nbr, SECTEUR_CONFIG_GSM)
#define configGSM_Read(pBuffer, Base, Nbr) config_Read(pBuffer, Base, Nbr, SECTEUR_CONFIG_GSM)

#define configGSM_String(Base) config_String(Base, SECTEUR_CONFIG_GSM)
#define configGSM_Char(Base) config_Char(Base, SECTEUR_CONFIG_GSM)
#define configGSM_Short(Base) config_Short(Base, SECTEUR_CONFIG_GSM)
#define configGSM_Long(Base) config_Long(Base, SECTEUR_CONFIG_GSM)
#define configGSM_Double(Base) config_Double(Base, SECTEUR_CONFIG_GSM)

#define configAFF_Write(pBuffer, Base, Nbr) config_Write(pBuffer, Base, Nbr, SECTEUR_CONFIG_AFF)
#define configAFF_Read(pBuffer, Base, Nbr) config_Read(pBuffer, Base, Nbr, SECTEUR_CONFIG_AFF)

#define configAFF_String(Base) config_String(Base, SECTEUR_CONFIG_AFF)
#define configAFF_Char(Base) config_Char(Base, SECTEUR_CONFIG_AFF)
#define configAFF_Short(Base) config_Short(Base, SECTEUR_CONFIG_AFF)
#define configAFF_Long(Base) config_Long(Base, SECTEUR_CONFIG_AFF)
#define configAFF_Double(Base) config_Double(Base, SECTEUR_CONFIG_AFF)

