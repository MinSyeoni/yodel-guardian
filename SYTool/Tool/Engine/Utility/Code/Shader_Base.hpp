matrix     g_matWorld;
matrix     g_matView;
matrix     g_matProj;

texture		g_BaseTexture;

sampler  BaseSampler = sampler_state
{
	texture = g_BaseTexture;

minfilter = linear;
magfilter = linear;
};