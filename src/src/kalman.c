//////////////////////////////////////////////////
// Date de création : 17/02/2010				//
// Auteur			: Faouzi					//
// Description		: Fichier Filtre kalman		//
//					  							//
//////////////////////////////////////////////////

double Echant_TabFiltre[2];
//-----------------------------------------------------------------------------------
// Fonction : FiltreKalman
// Utilité	: Calcule le filtre Kalman
// Entrée 	: Valeur a filtré, ancien valeur filtré, variation, 
// Sortie 	:	rien
//-----------------------------------------------------------------------------------
void FiltreKalman(double Val, double *ValOld, double *VariationOld, double Kalman_Q, double Kalman_R)
{
	*ValOld = ((*ValOld)+((*VariationOld+Kalman_Q)*(1/((*VariationOld+Kalman_Q)+Kalman_R)))*(Val-*ValOld));
	*VariationOld = ((1-((*VariationOld+Kalman_Q)*(1/(*VariationOld+Kalman_Q+Kalman_R))))*(*VariationOld+Kalman_Q));
}

