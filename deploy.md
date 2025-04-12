# Guide to deploy to Cloudflare Workers

1. Run wrangler service from compose.yaml

# Run in dev mode

1. Starting service should run wrangler in dev mode. Compose will expose it on port 80.

# Login 

1. Open bash in wrangler service
1. Run `npx wrangler login`
2. Open url in browser and login to Cloudflare
3. Cloudflare will try to open a localhost url (either on the browser or fetching it in the background). In that case, watch devtools.
4. Open a second bash in wrangler service
5. Run `curl "URL"` (don't forget the quotes)

# Deploy to production (must be logged in)

1. Ensure you are logged in to Cloudflare.
2. Open bash in wrangler service
3. Run `npx wrangler deploy`