#define	MAX_TYPE_DEBITMETRE_IRRIG					1					//Type Debimetre                       
#define	MIN_TYPE_DEBITMETRE_IRRIG                   0                                                          
#define	MAX_DEBIT_MINI_IRRIG                        500                 //Débit Min                            
#define	MIN_DEBIT_MINI_IRRIG                        0                                                          
#define	MAX_DEBIT_MAXI_IRRIG                        500                 //Débit Max                            
#define	MIN_DEBIT_MAXI_IRRIG                        0                                                          
#define	MAX_TEMPS_DEBIT_MINI_IRRIG                  0 //3600 s          //Temps Débit Min                      
#define	MIN_TEMPS_DEBIT_MINI_IRRIG                  0                                                          
#define	MAX_TEMPS_DEBIT_MAXI_IRRIG                  0 // PAS DE TEMPS DEBIT MAX A SUPPRIMER                //Temps Débit Max                      
#define	MIN_TEMPS_DEBIT_MAXI_IRRIG                  0                                                          
#define	MAX_DEBIT_SECURITE_IRRIG                    500                   //Débit Securite                       
#define	MIN_DEBIT_SECURITE_IRRIG                    0                                                          
#define	MAX_TEMPS_DEBIT_SECURITE_IRRIG              3600//S                   //Temps Débit Securite                 
#define	MIN_TEMPS_DEBIT_SECURITE_IRRIG              0 
#define	MAX_PRESS_REMPL_CANAL_IRRIG                 25                   //Pression Remplissage                 
#define	MIN_PRESS_REMPL_CANAL_IRRIG                 0                                                          
#define	MAX_TPS_REMPLI_CANALS_IRRIG                 900//min                   //Temps remplissage Canalisation       
#define	MIN_TPS_REMPLI_CANALS_IRRIG                 0                                                          
#define	MAX_PRESSION_MIN_RX_IRRIG                   25                   //Pression minimum Réseau              
#define	MIN_PRESSION_MIN_RX_IRRIG                   0                                                          
#define	MAX_TPS_PRESS_MIN_RX_IRRIG                  7200 //s                   //Temps minimum Réseau                 
#define	MIN_TPS_PRESS_MIN_RX_IRRIG                  0                                                          
#define	MAX_PRESSION_MIN_AMONT_IRRIG                25                   //Pression minimum Amont               
#define	MIN_PRESSION_MIN_AMONT_IRRIG                0                                                          
#define	MAX_HP_SECURITE_IRRIG                       25                   //HP Sécurité                          
#define	MIN_HP_SECURITE_IRRIG                       0.5
#define	MAX_CONSIGNE_PRESSION_SERVICE               25                   //Pression service                     
#define	MIN_CONSIGNE_PRESSION_SERVICE               0                                                          
#define	MAX_TOLERANCE_HAUT_PRESSION_SERVICE         25                   //Tolérance Haute                      
#define	MIN_TOLERANCE_HAUT_PRESSION_SERVICE         0                                                          
#define	MAX_TOLERANCE_BASSE_PRESSION_SERVICE        25                   //Tolérance Basse                      
#define	MIN_TOLERANCE_BASSE_PRESSION_SERVICE        0                                                          
#define	MAX_TPS_TOL_HAUTE_IRRIG                     600 //min                   //Temps tolérance Haute                
#define	MIN_TPS_TOL_HAUTE_IRRIG                     0                                                          
#define	MAX_TPS_TOL_BASSE_IRRIG                     600 //min                   //Temps tolérance Basse                
#define	MIN_TPS_TOL_BASSE_IRRIG                     0                                                          
#define	MAX_TPS_GESTION_PRESS_REMPLI_CANAL          600 //min                   //Temps Gestion 1                      
#define	MIN_TPS_GESTION_PRESS_REMPLI_CANAL          0                                                          
#define	MAX_TEMPS_DELTA_TOL_BASSE                   3600//s                   //Temps Delta Tolérance basse          
#define	MIN_TEMPS_DELTA_TOL_BASSE                   0                                                          
#define	MAX_HYSTERESIS_PRESSION_SERVICE             25                   //Hystérésis                           
#define	MIN_HYSTERESIS_PRESSION_SERVICE             0                                                          
#define	MAX_REGIME_MIN_SECURITE_PRINC_IRRIG         10000 //Tr_min                  //Régime Minimum                       
#define	MIN_REGIME_MIN_SECURITE_PRINC_IRRIG         0                                                          
#define	MAX_FREQUENCE_MIN_SECURITE_PRINC_IRRIG      50 // HZ                   //Fréquence Minimum                    
#define	MIN_FREQUENCE_MIN_SECURITE_PRINC_IRRIG      0                                                          
#define	MAX_REGIME_MAX_MOTEUR_SECURIT_IRRIG         10000 //Tr_min                   //Régime Maximum                       
#define	MIN_REGIME_MAX_MOTEUR_SECURIT_IRRIG         0                                                          
#define	MAX_FREQUENCE_MAX_MOTEUR_SECURIT_IRRIG      50 // HZ                   //Fréquence Maximum                    
#define	MIN_FREQUENCE_MAX_MOTEUR_SECURIT_IRRIG      0                                                          
#define	MAX_TPS_REGIME_MIN_MOTR_SECU_IRRIG          600 //min                   //Temps régime Minimum                 
#define	MIN_TPS_REGIME_MIN_MOTR_SECU_IRRIG          0                                                          
#define	MAX_TPS_FREQUENCE_MIN_MOTR_SECU_IRRIG       600 //min                   //Temps fréquence Minimum              
#define	MIN_TPS_FREQUENCE_MIN_MOTR_SECU_IRRIG       0                                                          
#define	MAX_TEMPS_MORT_IRRIG                        7200//s                   //Temps Mort                           
#define	MIN_TEMPS_MORT_IRRIG                        0                                                          
#define	MAX_SEUIL_TENSION_ALTER_IRRIG               20                   //Seuil Tension Alternateur            
#define	MIN_SEUIL_TENSION_ALTER_IRRIG               0                                                          
#define	MAX_RETARD_DEFAUT_DMR_IRRIG                 300//s                   //Temps retard Défauts                 
#define	MIN_RETARD_DEFAUT_DMR_IRRIG                 0                                                          
#define	MAX_TEMPS_FILTRE_E5                         3600//s                   //Filtre E5                            
#define	MIN_TEMPS_FILTRE_E5                         0                                                          
#define	MAX_TPS_ACC_AV_DMR_MOTEUR_IRRIG             60//s                   //Temps accélération Avant démarrage   
#define	MIN_TPS_ACC_AV_DMR_MOTEUR_IRRIG             0 

// gligt!!!!!!!!!!! 3yit!!!! regarde un autre esclave
                                                         
#define	MAX_SEUIL_TEMPERATURE      				    120                   //Ecart max température moteur         																																											
#define	MIN_SEUIL_TEMPERATURE			            0                                                          																																											
#define	MAX_TPS_ACC_REMPL_CANAL_IRRIG               180                   //Temps accélération Remplissage       																																											
#define	MIN_TPS_ACC_REMPL_CANAL_IRRIG               0                                                          																																											
#define	MAX_TPS_DECELERN_A_FOND_IRRIG               240                   //Temps décélération A fond            																																											
#define	MIN_TPS_DECELERN_A_FOND_IRRIG               0                                                          																																											
#define	MAX_TPS_ACCELERATION_IRRIG                  180                   //Temps accélération                   																																											
#define	MIN_TPS_ACCELERATION_IRRIG                  0                                                          																																											
#define	MAX_TPS_DECELERATION_IRRIG                  180                   //Temps décélération                   																																											
#define	MIN_TPS_DECELERATION_IRRIG                  0                                                          																																											
#define	MAX_TPS_ENTRE_2_ACTN_ACC_IRRIG              180                   //Temps entre actions accélération     																																											
#define	MIN_TPS_ENTRE_2_ACTN_ACC_IRRIG              0                                                          																																											
#define	MAX_TPS_ENTRE_2_ACTN_DECEL_IRRIG            180                   //Temps entre actions décélération     																																											
#define	MIN_TPS_ENTRE_2_ACTN_DECEL_IRRIG            0                                                          																																											
#define	MAX_NBR_IMPULS_ACC_REMPL_CANAL              4000                   //Accélération Remplissage canalisation																																											
#define	MIN_NBR_IMPULS_ACC_REMPL_CANAL              0                                                          																																											
#define	MAX_NBR_IMPULS_ACCEL_IRRIG                  4000                   //Accélération                         																																											
#define	MIN_NBR_IMPULS_ACCEL_IRRIG                  0                                                          																																											
#define	MAX_NBR_IMPULS_DECEL_IRRIG                  4000                   //Décélération                         																																											
#define	MIN_NBR_IMPULS_DECEL_IRRIG                  0                                                          																																											
#define	MAX_FREQ_POMPE_AUX_1                        50                   //Régime auxiliaire1                   																																											
#define	MIN_FREQ_POMPE_AUX_1                        0                                                          																																											
#define	MAX_FREQ_POMPE_AUX_2                        50                   //Régime auxiliaire2                   																																											
#define	MIN_FREQ_POMPE_AUX_2                        0                                                          																																											
#define	MAX_FREQ_POMPE_AUX_3                        50                   //Régime auxiliaire3                   																																											
#define	MIN_FREQ_POMPE_AUX_3                        0                                                          																																											
#define	MAX_REGIME_POMPE_AUX_1                      9000                   //Régime auxiliaire1                   																																											
#define	MIN_REGIME_POMPE_AUX_1                      0                                                          																																											
#define	MAX_REGIME_POMPE_AUX_2                      9000                   //Régime auxiliaire2                   																																											
#define	MIN_REGIME_POMPE_AUX_2                      0                                                          																																											
#define	MAX_REGIME_POMPE_AUX_3                      9000                   //Régime auxiliaire3                   																																											
#define	MIN_REGIME_POMPE_AUX_3                      0                                                          																																											
#define	MAX_DELTA_DEBIT_REGIME_DMR_POMPE_AUX        500                   //Delta régime démarrage               																																											
#define	MIN_DELTA_DEBIT_REGIME_DMR_POMPE_AUX        0                                                          																																											
#define	MAX_DELTA_DEBIT_FREQUENCE_DMR_POMPE_AUX     50                   //Delta fréquence démarrage            																																											
#define	MIN_DELTA_DEBIT_FREQUENCE_DMR_POMPE_AUX     0                                                          																																											
#define	MAX_DELTA_DEBIT_REGIME_ARRET_POMPE_AUX      500                   //Delta régime arrêt                   																																											
#define	MIN_DELTA_DEBIT_REGIME_ARRET_POMPE_AUX      0                                                          																																											
#define	MAX_DELTA_DEBIT_FREQUENCE_ARRET_POMPE_AUX   50                  //Delta fréquence arrêt                																																											
#define	MIN_DELTA_DEBIT_FREQUENCE_ARRET_POMPE_AUX   0                                                          																																											
#define	MAX_REGIME_MIN_SECURITE_AUX                 2500                   //Régime min auxiliaire                																																											
#define	MIN_REGIME_MIN_SECURITE_AUX                 0                                                          																																											
#define	MAX_FREQUENCE_MIN_SECURITE_AUX              50                   //Fréquence min auxiliaire             																																											
#define	MIN_FREQUENCE_MIN_SECURITE_AUX              0                                                          																																											
#define	MAX_PRSS_SERVICE_AUX_1                      25                   //Pression Service auxiliaire1         																																											
#define	MIN_PRSS_SERVICE_AUX_1                      -25
#define	MAX_PRSS_SERVICE_AUX_2                      25                   //Pression Service auxiliaire2         																																											
#define	MIN_PRSS_SERVICE_AUX_2                      -25
#define	MAX_PRSS_SERVICE_AUX_3                      25                   //Pression Service auxiliaire3         																																											
#define	MIN_PRSS_SERVICE_AUX_3                      -25
#define	MAX_TPS_GESTION_PRESS_AUTRES                60//MIN                   //Temps gestion 2                      																																											
#define	MIN_TPS_GESTION_PRESS_AUTRES                0                                                          																																											
#define	MAX_TEMPS_PERTE_OBJECTIF                    900//S                   //Temps Perte Objectif                 																																											
#define	MIN_TEMPS_PERTE_OBJECTIF                    0                                                          																																											
#define	MAX_TEMPS_DECEL_AV_DMR_AUX                  180                   //Temps Dec Avant démarrage auxiliaire 																																											
#define	MIN_TEMPS_DECEL_AV_DMR_AUX                  0                                                          																																											
#define	MAX_TEMPS_MINI_DMR_POMPE_AUX                0                   //Temps min entre démarrage            																																											
#define	MIN_TEMPS_MINI_DMR_POMPE_AUX                0                                                          																																											
#define	MAX_MAJ_INDEX_POMPE_AUX                     0                   //Mise a jour index auxiliaire         																																											
#define	MIN_MAJ_INDEX_POMPE_AUX                     0                                                          																																											
#define	MAX_DEBIT_POMPE_AUX_1                       500                   //Débit pompe auxiliaire 1             																																											
#define	MIN_DEBIT_POMPE_AUX_1                       0                                                          																																											
#define	MAX_DEBIT_POMPE_AUX_2                       500                   //Débit pompe auxiliaire 2             																																											
#define	MIN_DEBIT_POMPE_AUX_2                       0                                                          																																											
#define	MAX_DEBIT_POMPE_AUX_3                       500                   //Débit pompe auxiliaire 3             																																											
#define	MIN_DEBIT_POMPE_AUX_3                       0                                                          																																											
#define	MAX_HEURE_DEMARRE_CYCLE_IRRIG               0                   //Heure démarrage Cycle                																																											
#define	MIN_HEURE_DEMARRE_CYCLE_IRRIG               0                                                          																																											
#define	MAX_DUREE_CYCLE_IRRIG                       1000                   //Durée cycle                          																																											
#define	MIN_DUREE_CYCLE_IRRIG                       0                                                          																																											
#define	MAX_TYPE_MOTEUR_IRRIG                       0                   //Type Application                     																																											
#define	MIN_TYPE_MOTEUR_IRRIG                       0                                                          																																											
#define	MAX_DUREE_DEMARREUR_IRRIG     	            15                   //Durée demarreur                      																																											
#define	MIN_DUREE_DEMARREUR_IRRIG                   0                                                          																																											
#define	MAX_TEMPO_AVANT_DMR_CYCLE_IRRIG             500                   //Avant demarrage cycle                																																											
#define	MIN_TEMPO_AVANT_DMR_CYCLE_IRRIG             0                                                          																																											
#define	MAX_TEMPO_APRES_DMR_CYCLE_IRRIG             500                   //Aprés demarrage cycle                																																											
#define	MIN_TEMPO_APRES_DMR_CYCLE_IRRIG             0                                                          																																											
#define	MAX_TEMPO_AVANT_ARRET_CYCLE_IRRIG           500                   //Avant Arrêt cycle                    																																											
#define	MIN_TEMPO_AVANT_ARRET_CYCLE_IRRIG           0                                                          																																											
#define	MAX_TEMPO_APRES_ARRET_CYCLE_IRRIG           500                   //Aprés Arrêt cycle                    																																											
#define	MIN_TEMPO_APRES_ARRET_CYCLE_IRRIG           0                                                          																																											
#define	MAX_CHOIX_AUX_S5                            0                   //Choix Aux S5                         																																											
#define	MIN_CHOIX_AUX_S5                            0                                                          																																											
#define	MAX_NBR_POMPE_AUX                           3                   //Nombre auxiliaires                   																																											
#define	MIN_NBR_POMPE_AUX                           0                                                          																																											
#define	MAX_CHOIX_REGULATION_IRRIG                  1                   //Asservissement                       																																											
#define	MIN_CHOIX_REGULATION_IRRIG                  0                                                          																																											
#define	MAX_TEMPS_FILTRE_AUX                        120//S                   //Temps filtre auxiliaires             																																											
#define	MIN_TEMPS_FILTRE_AUX                        0                                                          																																											
#define	MAX_TPS_AVANT_DMR_POMPE_AUX                 600 //S                   //Temps avant démarrage pompe          																																											
#define	MIN_TPS_AVANT_DMR_POMPE_AUX                 0                                                          																																											
#define	MAX_TPS_APRES_DMR_POMPE_AUX                 600                   //Temps aprés démarrage pompe          																																											
#define	MIN_TPS_APRES_DMR_POMPE_AUX                 0                                                          																																											
#define	MAX_TPS_APRES_ARRET_POMPE_AUX               600                   //Temps aprés arrêt pompe              																																											
#define	MIN_TPS_APRES_ARRET_POMPE_AUX               0                                                          																																											
#define	MAX_TEMPS_ARRET_DERNIERE_POMPE_AUX          600                   //Temps aprés dernier arrêt            																																											
#define	MIN_TEMPS_ARRET_DERNIERE_POMPE_AUX          0                                                          																																											
#define	MAX_TEMPS_MAJ_FIFO_DEBIT                    60//S                   //Temps M.A.J FIFO                     																																											
#define	MIN_TEMPS_MAJ_FIFO_DEBIT                    0                                                          																																											
#define	MAX_TEMPS_APRES_OBJECTIF_1ER_DMR            600   //S                //Retard 1er démarrage                 																																											
#define	MIN_TEMPS_APRES_OBJECTIF_1ER_DMR            0                                                          																																											
#define	MAX_TEMPS_APRES_OBJECTIF_DMR_AUX            600                   //Retard apres démarrage               																																											
#define	MIN_TEMPS_APRES_OBJECTIF_DMR_AUX            0                                                          																																											
#define	MAX_TEMPS_APRES_OBJECTIF_ARRET_AUX          600   //S                //Retard aprés arrêt                   																																											
#define	MIN_TEMPS_APRES_OBJECTIF_ARRET_AUX          0                                                          																																											
#define	MAX_COEFF_DEBIT_IMPULSION                   0 //PAS DE LIMITE                  //Coefficient echantillonnage E2       //Coefficient echantillonnage E2                          																																											
#define	MIN_COEFF_DEBIT_IMPULSION                   0                                                                                                                    																																											
#define	MAX_TEMPS_MAX_ENTRE_IMP_E2                  0                   //Temps max E2                         //Temps max E2                                            																																											
#define	MIN_TEMPS_MAX_ENTRE_IMP_E2                  0                                                                                                                    																																											
#define	MAX_TEMPS_MIN_ENTRE_IMP_E2                  0                   //Temps max E2                         //Temps max E2                                            																																											
#define	MIN_TEMPS_MIN_ENTRE_IMP_E2                  0                                                                                                                    																																											
#define	MAX_NBR_IMP_E2								0                   //Temps max E2                         //Temps max E2                                            																																											
#define	MIN_NBR_IMP_E2                  			0                                                                                                                    																																											
#define	MAX_TEMP_AV_ACTIVATION_E3                   600   //100MS                //Temps avant activation E3            //Temps avant activation E3                               																																											
#define	MIN_TEMP_AV_ACTIVATION_E3                   0                                                                                                                    																																											
#define	MAX_TEMP_AV_ACTIVATION_E4                   600   //100MS                     //Temps avant activation E4            //Temps avant activation E4                               																																											
#define	MIN_TEMP_AV_ACTIVATION_E4                   0                                                                                                                    																																											
#define	MAX_TEMP_AV_ACTIVATION_E5                   600   //100MS                     //Temps avant activation E5            //Temps avant activation E5                               																																											
#define	MIN_TEMP_AV_ACTIVATION_E5                   0                                                                                                                    																																											
#define	MAX_COEFF_PRESSION_SERVICE                  99                   //Coefficient pression réseau          //Coefficient pression réseau                             																																											
#define	MIN_COEFF_PRESSION_SERVICE                  0                                                                                                                    																																											
#define	MAX_OFFSET_PRESSION_SERVICE                 99                   //Offset pression réseau               //Offset pression réseau                                  																																											
#define	MIN_OFFSET_PRESSION_SERVICE                 -99                                                                                                                    																																											
#define	MAX_COEFF_PRESSION_AMONT                    99                   //Coefficient pression amont           //Coefficient pression amont                              																																											
#define	MIN_COEFF_PRESSION_AMONT                    0                                                                                                                    																																											
#define	MAX_OFFSET_PRESSION_AMONT                   99                   //Offset pression amont                //Offset pression amont                                   																																											
#define	MIN_OFFSET_PRESSION_AMONT                   -99                                                                                                                    																																											
#define	MAX_COEFF_DEBIT_ANALOGIQUE                  99                   //Coefficient débit analogique         //Coefficient débit analogique                            																																											
#define	MIN_COEFF_DEBIT_ANALOGIQUE                  0                                                                                                                    																																											
#define	MAX_OFFSET_DEBIT_ANALOGIQUE                 99                   //Offset débit analogique              //Offset débit analogique                                 																																											
#define	MIN_OFFSET_DEBIT_ANALOGIQUE                 -99                                                                                                                    																																											
#define	MAX_PWM_ACCELERATION                        4000                   //PWM Accélération                     //PWM Accélération                                        																																											
#define	MIN_PWM_ACCELERATION                        0                                                                                                                    																																											
#define	MAX_PWM_DECELERATION                        4000                   //PWM Décélération                     //PWM Décélération                                        																																											
#define	MIN_PWM_DECELERATION                        0                                                                                                                    																																											
#define	MAX_VALEUR_SORTIE_PWM                       4000                   //Point Ana                            //Point Ana                                               																																											
#define	MIN_VALEUR_SORTIE_PWM                       0                                                                                                                    																																											
#define	MAX_REGIME_SIMULE_IRRIG                     10000                   //Régime Simule                        //Régime Simule                                           																																											
#define	MIN_REGIME_SIMULE_IRRIG                     0                                                                                                                    																																											
#define	MAX_FREQUENCE_SIMULE_IRRIG                  50                   //Fréquence Simule                     //Fréquence Simule                                        																																											
#define	MIN_FREQUENCE_SIMULE_IRRIG                  0                                                                                                                    																																											
#define	MAX_DUREE_RETROECLAIRAGE                    255//S                   //Durée Rétroéclairage                 //Durée Rétroéclairage                                    																																											
#define	MIN_DUREE_RETROECLAIRAGE                    0                                                          
#define	MAX_DATE_HEURE								0         //PAS DE MIN MAX                     
#define	MIN_DATE_HEURE								0                 //PAS DE MIN MAX   
#define	MAX_DELTA_PRESSION							25
#define	MIN_DELTA_PRESSION							0
#define	MAX_TEMPS_DELTA_PRESSION					120
#define	MIN_TEMPS_DELTA_PRESSION					0
#define	MAX_SEUIL_BAS								0
#define	MIN_SEUIL_BAS								0
#define	MAX_SEUIL_HAUT								0
#define	MIN_SEUIL_HAUT								0
#define	MAX_SEUIL_BAS_INTERMEDIAIRE					0
#define	MIN_SEUIL_BAS_INTERMEDIAIRE					0
#define	MAX_SEUIL_HAUT_INTERMEDIAIRE				0
#define	MIN_SEUIL_HAUT_INTERMEDIAIRE				0
#define	MAX_TEMPS_MORT_DMR_POMPE_RELEVAGE			600
#define	MIN_TEMPS_MORT_DMR_POMPE_RELEVAGE			0
#define	MAX_COEFF_TEMPERATURE_MOTEUR				0
#define	MIN_COEFF_TEMPERATURE_MOTEUR				0
#define	MAX_OFFSET_TEMPERATURE_MOTEUR				0
#define	MIN_OFFSET_TEMPERATURE_MOTEUR				0
#define	MAX_COEFF_TENSION_BATTERIE					0
#define	MIN_COEFF_TENSION_BATTERIE					0
#define	MAX_OFFSET_TENSION_BATTERIE					0
#define	MIN_OFFSET_TENSION_BATTERIE					0
#define	MAX_COEFF_FRQ_VARIATEUR						0
#define	MIN_COEFF_FRQ_VARIATEUR						0
#define	MAX_OFFSET_FRQ_VARIATEUR					0
#define	MIN_OFFSET_FRQ_VARIATEUR					0
#define	MAX_POINT_FREQ_DECEL_AUX					4000
#define	MIN_POINT_FREQ_DECEL_AUX					0
#define	MAX_DUREE_REARMEMENT_AUX					0
#define	MIN_DUREE_REARMEMENT_AUX					0
#define	MAX_TEMPS_DEBIT_MINI_AUX					0
#define	MIN_TEMPS_DEBIT_MINI_AUX					0
#define	MAX_TEMPS_PRECHAUFFAGE						0
#define	MIN_TEMPS_PRECHAUFFAGE						0
#define	MAX_TEMPS_ALTERNATEUR						0
#define	MIN_TEMPS_ALTERNATEUR						0
#define	MAX_TEMPS_ON_REMPLISSAGE_CANAL				30
#define	MIN_TEMPS_ON_REMPLISSAGE_CANAL				0.5
#define	MAX_TEMPS_OFF_REMPLISSAGE_CANAL				900
#define	MIN_TEMPS_OFF_REMPLISSAGE_CANAL				0
