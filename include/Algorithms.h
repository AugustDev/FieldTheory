#ifndef __ALGORITHMSP_
#define __ALGORITHMSP_

void FiniteDifference(CartesianGeometry PotentialG, CartesianGeometry PotentialG_new, Matrix B)
		for (int u = PotentialG.getInfX() + 1; u < PotentialG.getSupX(); u++)
		{
			for (int v = PotentialG.getInfY() + 1; v < PotentialG.getSupY(); v++)
			{
				PotentialG_new(u, v) = PotentialG(u, v) + CONFIG_DT / (CONFIG_DX * CONFIG_DY) * (PotentialG(u+1, v) - 4*PotentialG(u,v) + PotentialG(u-1, v) + PotentialG(u, v+1) + PotentialG(u, v-1) );
			}
		}

#endif // __ALGORITHMSP_